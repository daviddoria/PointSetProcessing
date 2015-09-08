// .NAME vtkPointSetNormalEstimation - Estimate normals of a point set using a local best fit plane.
// .SECTION Description
// At every point in the point set, vtkPointSetNormalEstimation computes the best
// fit plane of the set of points within a specified radius of the point (or a fixed number of neighbors).
// The normal of this plane is used as an estimate of the normal of the surface that would go through
// the points. The resulting normals are stored as a vtkFloatArray on the PointData of the output vtkPolyData.

#ifndef __vtkPointSetNormalEstimation_h
#define __vtkPointSetNormalEstimation_h

#include "vtkPolyDataAlgorithm.h" //superclass

class vtkPoints;
class vtkPlane;

class vtkPointSetNormalEstimation : public vtkPolyDataAlgorithm
{
  public:
    enum ModeEnum {FIXED_NUMBER, RADIUS};

    static vtkPointSetNormalEstimation *New();
    vtkTypeMacro(vtkPointSetNormalEstimation, vtkPolyDataAlgorithm);
    void PrintSelf(ostream &os, vtkIndent indent);

    // The number of neighbors to use to estimate the plane if MODE=FIXED_NUMBER
    vtkSetMacro(NumberOfNeighbors, unsigned int);
    vtkGetMacro(NumberOfNeighbors, unsigned int);

    // The radius to use to find neighbors to estimate the plane if MODE=RADIUS
    vtkSetMacro(Radius, float);
    vtkGetMacro(Radius, float);

    void SetModeToFixedNumber();
    void SetModeToRadius();

    int IterateEvent;

  protected:
    vtkPointSetNormalEstimation();
    ~vtkPointSetNormalEstimation() {};
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  private:
    unsigned int NumberOfNeighbors; // The number of neighbors to use in constructing the graph.
    float Radius;
    ModeEnum Mode;

};

// Helper functions
void BestFitPlane(vtkPoints* points, vtkPlane* bestPlane, vtkIdList* idsToUse);
void CenterOfMass(vtkPoints* points, double* center, vtkIdList* idsToUse);

void BestFitPlane(vtkPoints* points, vtkPlane* BestPlane);
void CenterOfMass(vtkPoints* points, double* Center);

#endif
