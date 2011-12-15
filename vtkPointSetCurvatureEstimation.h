// .NAME vtkPointSetCurvatureEstimation - Annotate a point set with an intuitive idea of "flatness" or "curvature".
// .SECTION Description
// vtkPointSetCurvatureEstimation computes the average distance to the plane specified by the normal of each point of kNeighbors of the point.
// As the curvature increases, the surface is locally less planar.

#ifndef __vtkPointSetCurvatureEstimation_h
#define __vtkPointSetCurvatureEstimation_h

#include <vtkSmartPointer.h>
#include "vtkPolyDataAlgorithm.h" //superclass

class vtkPoints;
class vtkPlane;

class vtkPointSetCurvatureEstimation : public vtkPolyDataAlgorithm
{

  public:
    static vtkPointSetCurvatureEstimation *New();
    vtkTypeMacro(vtkPointSetCurvatureEstimation, vtkPolyDataAlgorithm);
    void PrintSelf(ostream &os, vtkIndent indent);

    vtkGetMacro(Radius, float);
    vtkSetMacro(Radius, float);

    vtkGetMacro(UseAutoRadius, bool);
    vtkSetMacro(UseAutoRadius, bool);

    void SetAutoRadius(vtkPolyData* input);

  protected:
    vtkPointSetCurvatureEstimation();
    ~vtkPointSetCurvatureEstimation(){}

    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *); //the function that makes this class work with the vtk pipeline

  private:
    float Radius; // The radius of the sphere used to find neighbors to use in the local plane estimation.
    bool UseAutoRadius; // Specify if a radius should be automatically selected using a heuristic on the bounding box size. Default is true.
};

//void BestFitPlane(vtkPoints* points, vtkPlane* BestPlane);
void ComputeCenterOfMass(vtkPoints* points, double* Center);

#endif

