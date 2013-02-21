#include "PointSetProcessingWidget.h"

// Qt
#include <QFileDialog>
#include <QThread>

// VTK
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkHedgeHog.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

// Custom
#include "vtkPointSetNormalEstimation.h"
#include "vtkPointSetNormalOrientation.h"
#include "vtkPointSetNormalOrientationToPoint.h"

PointSetProcessingWidget::PointSetProcessingWidget(QMainWindow *parent)
{
  SharedConstructor();
};

PointSetProcessingWidget::PointSetProcessingWidget(const std::string& fileName)
{
  SharedConstructor();
  OpenFile(fileName);
}

void PointSetProcessingWidget::slot_ProgressUpdate(int value)
{
//   if(value % 10 == 0)
//     {
//     this->statusBar()->showMessage("Computing normal for point " + QString::number(value));
//     }
  //std::cout << "value: " << value << std::endl;
  this->ProgressDialog->setValue(value);
}

void PointSetProcessingWidget::SharedConstructor()
{
  this->setupUi(this);

  this->SphereSource = vtkSmartPointer<vtkSphereSource>::New();
  this->SphereSource->SetRadius(this->sldNeighborRadius->GetValue());
  this->SphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->SphereMapper->SetInputConnection(this->SphereSource->GetOutputPort());
  this->SphereActor = vtkSmartPointer<vtkActor>::New();
  this->SphereActor->SetMapper(this->SphereMapper);
  this->SphereActor->GetProperty()->SetOpacity(.2);

  this->PointsPolyData = vtkSmartPointer<vtkPolyData>::New();
  this->PointsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->PointsMapper->SetInputData(this->PointsPolyData);
  this->PointsActor = vtkSmartPointer<vtkActor>::New();
  this->PointsActor->SetMapper(this->PointsMapper);

  this->NormalEstimationFilter = vtkSmartPointer<vtkPointSetNormalEstimation>::New();
  this->NormalEstimationFilter->SetModeToRadius();
  this->NormalEstimationFilter->SetRadius(this->sldNeighborRadius->GetValue());
  this->NormalEstimationFilter->SetInputData(this->PointsPolyData);

  this->NormalEstimationThread = new QThread;
  this->NormalEstimationComputationObject = new VTKComputationThread<vtkPointSetNormalEstimation>;
  this->NormalEstimationComputationObject->SetFilter(this->NormalEstimationFilter);
  this->NormalEstimationComputationObject->moveToThread(this->NormalEstimationThread);

  // Normals
  this->NormalsPolyData = vtkSmartPointer<vtkPolyData>::New();

  // Setup the arrows
  this->HedgeHogFilter = vtkSmartPointer<vtkHedgeHog>::New();
  this->HedgeHogFilter->SetInputData(this->NormalsPolyData);
  this->HedgeHogFilter->SetVectorModeToUseNormal();
  this->HedgeHogFilter->SetScaleFactor(this->sldArrowSize->GetValue());
  this->HedgeHogFilter->Update();

  this->NormalsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->NormalsMapper->SetInputConnection(this->HedgeHogFilter->GetOutputPort());
  this->NormalsActor = vtkSmartPointer<vtkActor>::New();
  this->NormalsActor->SetMapper(this->NormalsMapper);
  this->NormalsActor->GetProperty()->SetInterpolationToFlat();

  // VTK Renderer
  this->Renderer = vtkSmartPointer<vtkRenderer>::New();
  this->Renderer->AddViewProp(this->PointsActor);
  this->Renderer->AddViewProp(this->NormalsActor);
  this->Renderer->AddViewProp(this->SphereActor);

  // Connect VTK with Qt
  this->qvtkWidget->GetRenderWindow()->AddRenderer(this->Renderer);

  // Marquee mode
  this->progressBar->setMinimum(0);
  this->progressBar->setMaximum(0);
  this->progressBar->hide();

  connect(this->NormalEstimationThread, SIGNAL(started()), this->NormalEstimationComputationObject, SLOT(start()));
  connect(this->NormalEstimationThread, SIGNAL(started()), this, SLOT(slot_StartProgressBar()));
  connect(this->NormalEstimationComputationObject, SIGNAL(finished()), this->NormalEstimationThread, SLOT(quit()));
  connect(this->NormalEstimationComputationObject, SIGNAL(progressUpdate(int)), this, SLOT(slot_ProgressUpdate(int)));
  connect(this->NormalEstimationComputationObject, SIGNAL(finished()), this, SLOT(slot_StopProgressBar()));
  connect(this->NormalEstimationComputationObject, SIGNAL(finished()), this, SLOT(slot_NormalEstimationComplete()));
}

void PointSetProcessingWidget::on_btnGenerateNormals_clicked()
{
  this->ProgressDialog = new QProgressDialog("Generating normals...", "Abort", 0, this->PointsPolyData->GetNumberOfPoints() - 1, this);
  connect(this->ProgressDialog, SIGNAL(canceled()), this, SLOT(slot_NormalEstimationCanceled()));
  this->ProgressDialog->setWindowModality(Qt::WindowModal);
  this->statusBar()->showMessage("Computing normals...");

  this->Timer.start();

  // Estimate normals
  this->NormalEstimationThread->start();
}

void PointSetProcessingWidget::on_btnOrientNormals_clicked()
{
  // Perform normal orientation
  vtkSmartPointer<vtkPointSetNormalOrientation> normalOrientationFilter = vtkSmartPointer<vtkPointSetNormalOrientation>::New();
  normalOrientationFilter->SetInputData(this->NormalsPolyData);
  normalOrientationFilter->SetKNearestNeighbors(10);
  normalOrientationFilter->Update();
  
  this->NormalsPolyData->DeepCopy(normalOrientationFilter->GetOutput());
  this->NormalsPolyData->Modified();

  this->qvtkWidget->GetRenderWindow()->Render();
}

void PointSetProcessingWidget::on_actionOpenFile_activated()
{
  QString fileName = QFileDialog::getOpenFileName(this, "OpenFile", ".", "VTP Files (*.vtp)");

  if(fileName.isEmpty())
    {
    return;
    }

  OpenFile(fileName.toStdString());

}

void PointSetProcessingWidget::OpenFile(const std::string& fileName)
{
  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(fileName.c_str());
  reader->Update();
  this->PointsPolyData->DeepCopy(reader->GetOutput());
  this->PointsPolyData->Modified();
  this->Renderer->ResetCamera();

  double p0[3];
  this->PointsPolyData->GetPoint(0,p0);
  this->SphereActor->SetPosition(p0);

  this->statusBar()->showMessage("Opened file with " + QString::number(this->PointsPolyData->GetNumberOfPoints()) + " points.");
}

void PointSetProcessingWidget::on_actionSave_activated()
{
  // Get a filename to save
  QString fileName = QFileDialog::getSaveFileName(this, "Save File", ".", "VTP Files (*.vtp)");

  if(fileName.toStdString().empty())
    {
    std::cerr << "File not saved." << std::endl;
    return;
    }

  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(fileName.toStdString().c_str());
  writer->SetInputData(this->NormalsPolyData);
  writer->Write();
}

void PointSetProcessingWidget::slot_StartProgressBar()
{
  this->progressBar->show();
}

void PointSetProcessingWidget::slot_StopProgressBar()
{
  this->progressBar->hide();
}

void PointSetProcessingWidget::slot_NormalEstimationComplete()
{
  std::cout << "Normal estimation completed in " << QTime().addMSecs(this->Timer.elapsed()).second() << " seconds." << std::endl;
  // std::cout << "slot_NormalEstimationComplete()" << std::endl;
  this->NormalsPolyData->DeepCopy(this->NormalEstimationFilter->GetOutput());
  this->NormalsPolyData->Modified();

  this->qvtkWidget->GetRenderWindow()->Render();
}

void PointSetProcessingWidget::slot_NormalEstimationCanceled()
{
  delete this->NormalEstimationThread;
}

void PointSetProcessingWidget::on_sldNeighborRadius_valueChanged(float value)
{
  // std::cout << "on_sldNeighborRadius_valueChanged to " << value << " (" << this->sldNeighborRadius->GetValue() << ")" << std::endl;
  this->NormalEstimationFilter->SetRadius(value);
  this->SphereSource->SetRadius(value);
  this->SphereSource->Update();
  this->qvtkWidget->GetRenderWindow()->Render();
}

void PointSetProcessingWidget::on_sldArrowSize_valueChanged(float value)
{
  // std::cout << "on_sldArrowSize_valueChanged to " << value << std::endl;
  this->HedgeHogFilter->SetScaleFactor(value);
  this->HedgeHogFilter->Update();
  this->qvtkWidget->GetRenderWindow()->Render();
}

void PointSetProcessingWidget::on_btnOrientNormalsToPoint_clicked()
{
  double orientationPoint[3];
  
  QVector<double> coord = this->coordOrientationPoint->GetCoordinate();
  orientationPoint[0] = coord[0];
  orientationPoint[1] = coord[1];
  orientationPoint[2] = coord[2];
  
  this->statusBar()->showMessage("Orienting to point " + QString::number(coord[0]) + " " + QString::number(coord[1]) + " " + QString::number(coord[2]));

  vtkSmartPointer<vtkPointSetNormalOrientationToPoint> normalOrientationToPointFilter = vtkSmartPointer<vtkPointSetNormalOrientationToPoint>::New();
  normalOrientationToPointFilter->SetInputData(this->NormalsPolyData);
  normalOrientationToPointFilter->SetOrientationPoint(orientationPoint);
  normalOrientationToPointFilter->Update();

  this->NormalsPolyData->DeepCopy(normalOrientationToPointFilter->GetOutput());
  this->NormalsPolyData->Modified();

  this->qvtkWidget->GetRenderWindow()->Render();
  
  this->statusBar()->showMessage("Finished orienting to point");
}
