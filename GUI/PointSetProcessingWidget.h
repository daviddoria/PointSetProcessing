#ifndef PointSetProcessingWidget_H
#define PointSetProcessingWidget_H

#include "ui_PointSetProcessingWidget.h"

// Qt
#include <QMainWindow>
#include <QTime>
#include <QProgressDialog>

// VTK
#include <vtkSmartPointer.h>
class vtkActor;
class vtkHedgeHog;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkSphereSource;

// Custom
#include "VTKComputationThread.h"
class vtkPointSetNormalEstimation;

class PointSetProcessingWidget : public QMainWindow, public Ui::PointSetProcessingWidget
{
Q_OBJECT
public:

  PointSetProcessingWidget(QMainWindow *parent = 0);
  PointSetProcessingWidget(const std::string& fileName);
  void SharedConstructor();

  void OpenFile(const std::string& fileName);

public slots:

  void on_btnOrientNormalsToPoint_clicked();

  void on_btnGenerateNormals_clicked();
  void on_btnOrientNormals_clicked();

  void on_actionOpenFile_activated();
  void on_actionSave_activated();

  void slot_StartProgressBar();
  void slot_StopProgressBar();

  void slot_NormalEstimationComplete();
  void slot_NormalEstimationCanceled();

  void on_sldNeighborRadius_valueChanged(float);
  void on_sldArrowSize_valueChanged(float);

  void slot_ProgressUpdate(int);

private:
  vtkSmartPointer<vtkPolyData> PointsPolyData;
  vtkSmartPointer<vtkPolyDataMapper> PointsMapper;
  vtkSmartPointer<vtkActor> PointsActor;

  // After every filtering operation, the result is deep copied into NormalsPolyData
  // and therefore automatically displayed since the pipeline is already setup.
  vtkSmartPointer<vtkPolyData> NormalsPolyData;
  vtkSmartPointer<vtkPolyDataMapper> NormalsMapper;
  vtkSmartPointer<vtkActor> NormalsActor;
  vtkSmartPointer<vtkHedgeHog> HedgeHogFilter;

  vtkSmartPointer<vtkRenderer> Renderer;

  // This sphere indicates the radius used for the neighbor search
  vtkSmartPointer<vtkSphereSource> SphereSource;
  vtkSmartPointer<vtkPolyDataMapper> SphereMapper;
  vtkSmartPointer<vtkActor> SphereActor;

  vtkSmartPointer<vtkPointSetNormalEstimation> NormalEstimationFilter;
  VTKComputationThread<vtkPointSetNormalEstimation>* NormalEstimationComputationObject;
  QThread* NormalEstimationThread;

  QTime Timer;

  QProgressDialog* ProgressDialog;
};

#endif
