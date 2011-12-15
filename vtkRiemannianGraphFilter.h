/*
This class takes in a vtkDataSet, computes the Euclidean Minimum Spanning Tree on the points, then adds edges from each point to its kNeighbors nearest neighbors.
*/

#ifndef __vtkRiemannianGraphFilter_h
#define __vtkRiemannianGraphFilter_h

#include <vtkSmartPointer.h>
#include "vtkGraphAlgorithm.h" //superclass

class vtkTree;
class vtkMutableUndirectedGraph;

class vtkRiemannianGraphFilter : public vtkGraphAlgorithm
{
  public:
    static vtkRiemannianGraphFilter *New();
    vtkTypeMacro(vtkRiemannianGraphFilter, vtkGraphAlgorithm);
    void PrintSelf(ostream &os, vtkIndent indent);

    vtkSetMacro(kNeighbors, unsigned int);
    vtkGetMacro(kNeighbors, unsigned int);

  protected:
    vtkRiemannianGraphFilter();
    ~vtkRiemannianGraphFilter(){}
    int FillInputPortInformation( int port, vtkInformation* info );
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *); //the function that makes this class work with the vtk pipeline
    int RequestDataObject(vtkInformation *, vtkInformationVector **, vtkInformationVector *); //the function that makes this class work with the vtk pipeline

  private:
    unsigned int kNeighbors;
};

//Helper functions
void TreeToUndirectedGraph(vtkTree* Tree, vtkMutableUndirectedGraph* UndirectedGraph);

#endif

