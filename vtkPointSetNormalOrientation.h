// .NAME vtkPointSetNormalOrientation - Orient the normals of a point set
// consistently.
// .SECTION Description
// This filter implements the technique described by Hoppe et al.
// in ``Surface reconstruction from unorganized points''. This method
// constructs a nearest neighbor graph on the points. As a good guess
// at ``outside'', the technique finds the point with the largest z
// value and adjusts that point's normal to point more toward the positive
// z direction. This initial normal direction is propagated over a minimum
// spanning tree computed over the graph.

#ifndef __vtkPointSetNormalOrientation_h
#define __vtkPointSetNormalOrientation_h

#include <vtkSmartPointer.h>
#include <vtkMutableUndirectedGraph.h>

#include "vtkPolyDataAlgorithm.h" //superclass

class vtkPoints;
class vtkPlane;
class vtkIntArray;
class vtkPolyData;

class vtkPointSetNormalOrientation : public vtkPolyDataAlgorithm
{
  public:
    // Define graph filter types
    static const unsigned int RIEMANN_GRAPH = 0;
    static const unsigned int KNN_GRAPH = 1;

    static vtkPointSetNormalOrientation *New();
    vtkTypeMacro(vtkPointSetNormalOrientation, vtkPolyDataAlgorithm);
    void PrintSelf(ostream &os, vtkIndent indent);

    vtkSetMacro(KNearestNeighbors, unsigned int);
    vtkGetMacro(KNearestNeighbors, unsigned int);
    vtkSetMacro(GraphFilterType, unsigned int);
    vtkGetMacro(GraphFilterType, unsigned int);

    int IterateEvent;
    int ProgressEvent;

  protected:
    vtkPointSetNormalOrientation();
    ~vtkPointSetNormalOrientation(){}
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *); //the function that makes this class work with the vtk pipeline

  private:

    unsigned int KNearestNeighbors; //this is used to determine how many neighbors are connected in the graph
    unsigned int GraphFilterType; // wether to use the vtkRiemannianGraphFilter (default) or the vtkKNNGraphFilter

};


void MultiplyScalar(double a[3], const double s);
unsigned int FindMaxZId(vtkPolyData* input);

#endif

