#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkDataSet.h>
#include <vtkKdTreePointLocator.h>
#include <vtkMath.h>
#include <vtkGraph.h>
#include <vtkTree.h>
#include <vtkMutableUndirectedGraph.h>
#include <vtkAdjacentVertexIterator.h>
#include <vtkEdgeListIterator.h>

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h> //for new() macro
#include <vtkExecutive.h>

#include "vtkKNNGraphFilter.h"

vtkStandardNewMacro(vtkKNNGraphFilter);

vtkKNNGraphFilter::vtkKNNGraphFilter()
{
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
  
  this->KNeighbors = 5;
}

int vtkKNNGraphFilter::RequestDataObject(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector)
{
  vtkUndirectedGraph* output = 0;
  output = vtkUndirectedGraph::New();
    
  this->GetExecutive()->SetOutputData(0, output);
  output->Delete();

  return 1;
}

int vtkKNNGraphFilter::RequestData(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector)
{
   // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and ouptut
  vtkPointSet* input = vtkPointSet::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  
  vtkGraph* output = vtkGraph::SafeDownCast(
                                            outInfo->Get(vtkDataObject::DATA_OBJECT()));
  
  vtkSmartPointer<vtkMutableUndirectedGraph> g = 
      vtkSmartPointer<vtkMutableUndirectedGraph>::New();
  
  //create a vector to store vertex IDs
  std::vector<vtkIdType> vertices(input->GetNumberOfPoints());
  
  //add a vertex for every point
  for(unsigned int i = 0; i < input->GetNumberOfPoints(); i++)
    {
    vertices[i] = g->AddVertex();
    }
  
  //create the edge weight array
  vtkSmartPointer<vtkDoubleArray> weights = 
      vtkSmartPointer<vtkDoubleArray>::New();
  weights->SetNumberOfComponents(1);
  weights->SetName("Weights");
    
  //create a KDTree of the points
  vtkSmartPointer<vtkKdTreePointLocator> kdTree = 
      vtkSmartPointer<vtkKdTreePointLocator>::New();
  kdTree->SetDataSet(input);
  kdTree->BuildLocator();
  
  //find the nearest neighbors to each point and add adges between them, if they do not already exist and they are not self loops
  for(unsigned int pointID = 0; pointID < input->GetNumberOfPoints(); pointID++)
    {
    if(pointID % 1000 == 0)
      cout << "pointID: " << pointID << " out of " << input->GetNumberOfPoints() << endl;
    
    double point[3];
    input->GetPoint(pointID, point);
    vtkSmartPointer<vtkIdList> result = 
        vtkSmartPointer<vtkIdList>::New();
  
    kdTree->FindClosestNPoints(this->KNeighbors + 1, point, result);
  
    for(int neighbor = 1; neighbor < this->KNeighbors + 1; neighbor++) //start at 1 and go to (kNeighbors+1) so we don't include the exact same point and still use n neighbors
      {
      unsigned int neighborID = result->GetId(neighbor);
      //cout << "neighborID: " << neighborID << endl;
      vtkSmartPointer<vtkAdjacentVertexIterator> iterator = 
          vtkSmartPointer<vtkAdjacentVertexIterator>::New();
      g->GetAdjacentVertices(pointID, iterator);
      bool edgeAlreadyExists = false;
      while(iterator->HasNext())
        {
        if(iterator->Next() == neighborID)
          {
          edgeAlreadyExists = true;
          break;
          }
        }
      if(!edgeAlreadyExists && (pointID != neighborID))
        {
        g->AddEdge(pointID, neighborID);
        
        //set the edge weight to the distance between the points
        double neighborPoint[3];
        input->GetPoint(neighborID, neighborPoint);
        
        double w = sqrt(vtkMath::Distance2BetweenPoints(point, neighborPoint));
        //cout << "Distance: " << w << endl;
        weights->InsertNextValue(w);
        
        }
      }
    }
  
  //add the point coordinates to the graph
  g->SetPoints(input->GetPoints());
  
  //add the edge weight array to the graph
  g->GetEdgeData()->AddArray(weights);
    
  //set the output of the filter
  output->ShallowCopy(g);
  
  //why doesn't MST preserve points??
  output->SetPoints(input->GetPoints());
  
  return 1;
}

int vtkKNNGraphFilter::FillInputPortInformation( int port, vtkInformation* info )
{
  if (port == 0)
    {
    info->Set( vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPointSet" );
    return 1;
    }

  return 0;
}

void vtkKNNGraphFilter::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
