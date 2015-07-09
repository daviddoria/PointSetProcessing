/*
This class takes in a vtkDataSet and creates a graph with every point connected to its K nearest neighbors. The class will optionally connect disconnected regions of the graph by setting the ConnectRegions variable to true.
*/

#ifndef __vtkKNNGraphFilter_h
#define __vtkKNNGraphFilter_h

#include <vtkSmartPointer.h>
#include "vtkGraphAlgorithm.h" //superclass

class vtkTree;
class vtkMutableUndirectedGraph;

class vtkKNNGraphFilter : public vtkGraphAlgorithm
{
  public:
    static vtkKNNGraphFilter *New();
    vtkTypeMacro(vtkKNNGraphFilter, vtkGraphAlgorithm);
    void PrintSelf(ostream &os, vtkIndent indent);
    
    //How many neighbors to connect each point to 
    vtkSetMacro(KNeighbors, unsigned int);
    vtkGetMacro(KNeighbors, unsigned int);
  
  protected:
    vtkKNNGraphFilter();
    ~vtkKNNGraphFilter() {};
    int FillInputPortInformation( int port, vtkInformation* info );
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *); //the function that makes this class work with the vtk pipeline
    int RequestDataObject(vtkInformation *, vtkInformationVector **, vtkInformationVector *); //the function that makes this class work with the vtk pipeline
    
  private:

    unsigned int KNeighbors;
    
};

#endif

