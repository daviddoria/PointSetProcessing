// .NAME vtkPointSetNormalEstimation - Estimate normals of a point set using a local best fit plane.
// .SECTION Description
// At every point in the point set, vtkPointSetNormalEstimation computes the best 
// fit plane of the set of points within a specified radius of the point. The normal
// of this plane is used as an estimate of the normal of the surface that would go through
// the points.

#ifndef __vtkPointSetNormalEstimation_h
#define __vtkPointSetNormalEstimation_h

#include "vtkPolyDataAlgorithm.h" //superclass

class vtkPoints;
class vtkPlane;

class vtkPointSetNormalEstimation : public vtkPolyDataAlgorithm
{

  public:
    static vtkPointSetNormalEstimation *New();
    vtkTypeMacro(vtkPointSetNormalEstimation, vtkPolyDataAlgorithm);
    void PrintSelf(ostream &os, vtkIndent indent);
    
    //The number of neighbors to use in constructing the graph.
    vtkSetMacro(kNeighbors, unsigned int);
    vtkGetMacro(kNeighbors, unsigned int);
    
  protected:
    vtkPointSetNormalEstimation();
    ~vtkPointSetNormalEstimation() {};
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
    
  private:
    unsigned int kNeighbors; //The number of neighbors to use in constructing the graph.
};

//Helper functions - these should be included in VTK soon.
void BestFitPlane(vtkPoints *points, vtkPlane *BestPlane);
void CenterOfMass(vtkPoints* points, double* Center);

#endif
