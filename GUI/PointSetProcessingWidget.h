#ifndef PointSetProcessingWidget_H
#define PointSetProcessingWidget_H

#include "ui_PointSetProcessingWidget.h"

// Qt
#include <QMainWindow>

// VTK
#include <vtkSmartPointer.h>
class vtkPolyData;
class vtkPolyDataMapper;
class vtkActor;
class vtkGlyph3D;
class vtkArrowSource;

//class VTKComputationThread;
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
  
  vtkSmartPointer<vtkPointSetNormalEstimation> NormalEstimationFilter;
  VTKComputationThread<vtkPointSetNormalEstimation>* NormalEstimationThread;
};

#endif
