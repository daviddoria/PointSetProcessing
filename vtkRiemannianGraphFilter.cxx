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

#include "vtkEuclideanMinimumSpanningTree.h"
#include "vtkRiemannianGraphFilter.h"

vtkStandardNewMacro(vtkRiemannianGraphFilter);

vtkRiemannianGraphFilter::vtkRiemannianGraphFilter()
{
  this->kNeighbors = 4;
  
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
}

int vtkRiemannianGraphFilter::RequestDataObject(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector)
{
  vtkUndirectedGraph* output = 0;
  output = vtkUndirectedGraph::New();
    
  this->GetExecutive()->SetOutputData(0, output);
  output->Delete();

  return 1;
}

int vtkRiemannianGraphFilter::RequestData(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and ouptut
  //vtkDataSet* input = vtkDataSet::SafeDownCast(
      ///inInfo->Get(vtkDataObject::DATA_OBJECT()));
  
  vtkPolyData* input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  
  
  vtkGraph *output = vtkGraph::SafeDownCast(
                                            outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkSmartPointer<vtkEuclideanMinimumSpanningTree> EMSTFilter = vtkSmartPointer<vtkEuclideanMinimumSpanningTree>::New();
  EMSTFilter->SetInput(input);
  EMSTFilter->Update();
  
  vtkTree* EMST = EMSTFilter->GetOutput();
  
  //copy EMST into a mutable graph
  /*
  //this doesn't work because the tree is directed and the MUG is undirected
  vtkMutableUndirectedGraph* NearestNeighborGraph = vtkMutableUndirectedGraph::New();
  NearestNeighborGraph->ShallowCopy(NearestNeighborGraphFilter->GetOutput());
  */

  vtkSmartPointer<vtkMutableUndirectedGraph> G = vtkSmartPointer<vtkMutableUndirectedGraph>::New();
  
  TreeToUndirectedGraph(EMST,G);
 
  vtkstd::cout << "G NumVertices: " << G->GetNumberOfVertices() << vtkstd::endl;
  
  //create a KDTree of the points
  vtkSmartPointer<vtkKdTreePointLocator> KDTree = vtkSmartPointer<vtkKdTreePointLocator>::New();
  KDTree->SetDataSet(input);
  KDTree->BuildLocator();
    
  //create the new edge weight array
  vtkSmartPointer<vtkDoubleArray> Weights = vtkSmartPointer<vtkDoubleArray>::New();
  Weights->SetNumberOfComponents(1);
  Weights->SetName("Weights");
    
  //find the nearest neighbors to each point and add adges between them, if they do not already exist and they are not self loops
  for(unsigned int pointID = 0; pointID < input->GetNumberOfPoints(); pointID++)
  {
    //vtkstd::cout << "pointID: " << pointID << vtkstd::endl;
    
    double point[3];
    input->GetPoint(pointID, point);
    vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
  
    KDTree->FindClosestNPoints(this->kNeighbors + 1, point, result);
  
    for(int neighbor = 1; neighbor < this->kNeighbors + 1; neighbor++) //start at 1 and go to (kNeighbors+1) so we don't include the exact same point and still use n neighbors
    {
      unsigned int neighborID = result->GetId(neighbor);
      //vtkstd::cout << "neighborID: " << neighborID << vtkstd::endl;
      vtkSmartPointer<vtkAdjacentVertexIterator> iterator = vtkSmartPointer<vtkAdjacentVertexIterator>::New();
      G->GetAdjacentVertices(pointID, iterator);
      bool EdgeAlreadyExists = false;
      while(iterator->HasNext())
      {
        if(iterator->Next() == neighborID)
        {
          EdgeAlreadyExists = true;
          break;
        }
      }
      if(!EdgeAlreadyExists && (pointID != neighborID))
      {
        G->AddEdge(pointID, neighborID);
      }
    }
  }
  
  //add the point coordinates to the graph
  G->SetPoints(input->GetPoints());
  
  output->ShallowCopy(G);
  
  return 1;
}

int vtkRiemannianGraphFilter::FillInputPortInformation( int port, vtkInformation* info )
{

  //The input should be a vtkDataSet
  if (port == 0)
  {
    //info->Set( vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet" );
    info->Set( vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData" );
    return 1;
  }

  return 0;
}

void vtkRiemannianGraphFilter::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  
  os << indent << "kneighbors: " << this->kNeighbors << vtkstd::endl;
  
}

void TreeToUndirectedGraph(vtkTree* Tree, vtkMutableUndirectedGraph* UndirectedGraph)
{
//add vertices to MUG
  for(unsigned int i = 0; i < Tree->GetNumberOfVertices(); i++)
  {
    UndirectedGraph->AddVertex();
  }
    
  vtkSmartPointer<vtkEdgeListIterator> EdgeIterator = vtkSmartPointer<vtkEdgeListIterator>::New();
  Tree->GetEdges(EdgeIterator);
  while(EdgeIterator->HasNext())
  {
    vtkEdgeType Edge = EdgeIterator->Next();
    //std::cout << "Source: " << Edge.Source << " Target: " << Edge.Target << vtkstd::endl;
    UndirectedGraph->AddEdge(Edge.Source, Edge.Target);
  }
  
  UndirectedGraph->SetPoints(Tree->GetPoints());
}
