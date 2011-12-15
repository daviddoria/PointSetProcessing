// .NAME vtkPointSetNormalOrientationToPoint - Orient the normals of a point set
// so that they are all "facing" a specified point.
// .SECTION Description
// This technique is useful for orienting the normals of the pointset produced by a stationary, single viewpoint LiDAR scanner.

#ifndef __vtkPointSetNormalOrientationToPoint_h
#define __vtkPointSetNormalOrientationToPoint_h

#include <vtkSmartPointer.h>

#include "vtkPolyDataAlgorithm.h" //superclass

class vtkPoints;
class vtkPolyData;

class vtkPointSetNormalOrientationToPoint : public vtkPolyDataAlgorithm
{
public:
  static vtkPointSetNormalOrientationToPoint *New();
  vtkTypeMacro(vtkPointSetNormalOrientationToPoint, vtkPolyDataAlgorithm);
  void PrintSelf(ostream &os, vtkIndent indent);

  vtkSetVector3Macro(OrientationPoint,double);
  vtkGetVector3Macro(OrientationPoint,double);

protected:
  vtkPointSetNormalOrientationToPoint();
  ~vtkPointSetNormalOrientationToPoint(){}
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *); //the function that makes this class work with the vtk pipeline

private:
  double OrientationPoint[3];
};

#endif
