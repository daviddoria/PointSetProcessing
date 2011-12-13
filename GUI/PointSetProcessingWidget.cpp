#include "PointSetProcessingWidget.h"

// Qt
#include <QFileDialog>

// VTK
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>

// Custom
#include "vtkPointSetNormalEstimation.h"

PointSetProcessingWidget::PointSetProcessingWidget(QMainWindow *parent)
{
  SharedConstructor();
};

PointSetProcessingWidget::PointSetProcessingWidget(const std::string& fileName)
{
  SharedConstructor();
  OpenFile(fileName);
}

void PointSetProcessingWidget::SharedConstructor()
{
  this->setupUi(this);

  this->NormalEstimationThread = new VTKComputationThread<vtkPointSetNormalEstimation>;
  this->NormalEstimationFilter = vtkSmartPointer<vtkPointSetNormalEstimation>::New();
  
  this->PointsPolyData = vtkSmartPointer<vtkPolyData>::New();
  this->PointsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->PointsMapper->SetInputConnection(this->PointsPolyData->GetProducerPort());
  this->PointsActor = vtkSmartPointer<vtkActor>::New();
  this->PointsActor->SetMapper(this->PointsMapper);
  
  // Normals
  this->NormalsPolyData = vtkSmartPointer<vtkPolyData>::New();
  
  // Setup the arrows
  this->ArrowSource = vtkSmartPointer<vtkArrowSource>::New();
  this->ArrowSource->Update();
  this->ArrowGlyphFilter = vtkSmartPointer<vtkGlyph3D>::New();
  this->ArrowGlyphFilter->SetSource(this->ArrowSource->GetOutput());
  this->ArrowGlyphFilter->OrientOn();
  this->ArrowGlyphFilter->SetScaleModeToScaleByScalar();
  this->ArrowGlyphFilter->SetScaleFactor(.1f);
  this->ArrowGlyphFilter->SetVectorModeToUseNormal();
  this->ArrowGlyphFilter->SetInputConnection(this->NormalsPolyData->GetProducerPort());
  this->ArrowGlyphFilter->Update();
  
  this->NormalsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->NormalsMapper->SetInputConnection(this->ArrowGlyphFilter->GetOutputPort());
  this->NormalsActor = vtkSmartPointer<vtkActor>::New();
  this->NormalsActor->SetMapper(this->NormalsMapper);
  
  // VTK Renderer
  this->Renderer = vtkSmartPointer<vtkRenderer>::New();
  this->Renderer->AddViewProp(this->PointsActor);
  this->Renderer->AddViewProp(this->NormalsActor);

  // Connect VTK with Qt
  this->qvtkWidget->GetRenderWindow()->AddRenderer(this->Renderer);

  // Marquee mode
  this->progressBar->setMinimum(0);
  this->progressBar->setMaximum(0);
  this->progressBar->hide();

  connect(this->NormalEstimationThread, SIGNAL(StartProgressBarSignal()), this, SLOT(slot_StartProgressBar()));
  connect(this->NormalEstimationThread, SIGNAL(StopProgressBarSignal()), this, SLOT(slot_StopProgressBar()));
  connect(this->NormalEstimationThread, SIGNAL(StopProgressBarSignal()), this, SLOT(slot_NormalEstimationComplete()));
}

void PointSetProcessingWidget::on_btnGenerateNormals_clicked()
{
  // Estimate normals
  this->NormalEstimationFilter->SetInputConnection(this->PointsPolyData->GetProducerPort());
  this->NormalEstimationThread->SetFilter(this->NormalEstimationFilter);
  this->NormalEstimationThread->start();
}

void PointSetProcessingWidget::on_btnOrientNormals_clicked()
{
  
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
  writer->SetInputConnection(this->NormalEstimationFilter->GetOutputPort());
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
  std::cout << "slot_NormalEstimationComplete()" << std::endl;
  this->NormalsPolyData->DeepCopy(this->NormalEstimationFilter->GetOutput());
  this->NormalsPolyData->Modified();
  this->qvtkWidget->GetRenderWindow()->Render();
}
