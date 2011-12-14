#ifndef PointSetProcessingWidget_H
#define PointSetProcessingWidget_H

#include "ui_PointSetProcessingWidget.h"

// Qt
#include <QMainWindow>

// VTK
#include <vtkSmartPointer.h>
class vtkActor;
class vtkArrowSource;
class vtkGlyph3D;
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

  void on_btnGenerateNormals_clicked();
  void on_btnOrientNormals_clicked();

  void on_actionOpenFile_activated();
  void on_actionSave_activated();

  void slot_StartProgressBar();
  void slot_StopProgressBar();

  void slot_NormalEstimationComplete();

  void on_sldNeighborRadius_valueChanged(float);
  void on_sldArrowSize_valueChanged(float);

private:
  vtkSmartPointer<vtkPolyData> PointsPolyData;
  vtkSmartPointer<vtkPolyDataMapper> PointsMapper;
  vtkSmartPointer<vtkActor> PointsActor;
  
  vtkSmartPointer<vtkPolyData> NormalsPolyData;
  vtkSmartPointer<vtkPolyDataMapper> NormalsMapper;
  vtkSmartPointer<vtkActor> NormalsActor;
  vtkSmartPointer<vtkArrowSource> ArrowSource;
  vtkSmartPointer<vtkGlyph3D> ArrowGlyphFilter;
  
  vtkSmartPointer<vtkRenderer> Renderer;
  
  // This sphere indicates the radius used for the neighbor search
  vtkSmartPointer<vtkSphereSource> SphereSource;
  vtkSmartPointer<vtkPolyDataMapper> SphereMapper;
  vtkSmartPointer<vtkActor> SphereActor;
    
  vtkSmartPointer<vtkPointSetNormalEstimation> NormalEstimationFilter;
  VTKComputationThread<vtkPointSetNormalEstimation>* NormalEstimationThread;
};

#endif
