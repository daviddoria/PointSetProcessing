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
  // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and ouptut
  //vtkDataSet* input = vtkDataSet::SafeDownCast(
      ///inInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkPolyData* input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));


  vtkGraph *output = vtkGraph::SafeDownCast(
                                            outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkSmartPointer<vtkEuclideanMinimumSpanningTree> emstFilter = vtkSmartPointer<vtkEuclideanMinimumSpanningTree>::New();
  emstFilter->SetInputData(input);
  emstFilter->Update();

  vtkTree* emst = emstFilter->GetOutput();

  //copy EMST into a mutable graph
  /*
  //this doesn't work because the tree is directed and the MUG is undirected
  vtkMutableUndirectedGraph* NearestNeighborGraph = vtkMutableUndirectedGraph::New();
  NearestNeighborGraph->ShallowCopy(NearestNeighborGraphFilter->GetOutput());
  */

  vtkSmartPointer<vtkMutableUndirectedGraph> g = vtkSmartPointer<vtkMutableUndirectedGraph>::New();

  TreeToUndirectedGraph(emst, g);

  std::cout << "G NumVertices: " << g->GetNumberOfVertices() << std::endl;

  // Create a KDTree of the points
  vtkSmartPointer<vtkKdTreePointLocator> kdTree = vtkSmartPointer<vtkKdTreePointLocator>::New();
  kdTree->SetDataSet(input);
  kdTree->BuildLocator();

  // Create the new edge weight array
  vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
  weights->SetNumberOfComponents(1);
  weights->SetName("Weights");

  // Find the nearest neighbors to each point and add adges between them, if they do not already exist and they are not self loops
  for(vtkIdType pointID = 0; pointID < input->GetNumberOfPoints(); pointID++)
    {
    // std::cout << "pointID: " << pointID << std::endl;

    double point[3];
    input->GetPoint(pointID, point);
    vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();

    kdTree->FindClosestNPoints(this->kNeighbors + 1, point, result);

    // Start at 1 and go to (kNeighbors+1) so we don't include the exact same point and still use n neighbors
    for(unsigned int neighbor = 1; neighbor < this->kNeighbors + 1; neighbor++)
      {
      vtkIdType neighborID = result->GetId(neighbor);
      //std::cout << "neighborID: " << neighborID << std::endl;
      vtkSmartPointer<vtkAdjacentVertexIterator> iterator = vtkSmartPointer<vtkAdjacentVertexIterator>::New();
      g->GetAdjacentVertices(pointID, iterator);
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
        g->AddEdge(pointID, neighborID);
        }
      }
    }

  // Add the point coordinates to the graph
  g->SetPoints(input->GetPoints());

  output->ShallowCopy(g);

  return 1;
}

int vtkRiemannianGraphFilter::FillInputPortInformation( int port, vtkInformation* info )
{
  // The input should be a vtkDataSet
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

  os << indent << "kneighbors: " << this->kNeighbors << std::endl;
}

void TreeToUndirectedGraph(vtkTree* tree, vtkMutableUndirectedGraph* undirectedGraph)
{
  // Add vertices to MUG
  for(vtkIdType i = 0; i < tree->GetNumberOfVertices(); i++)
    {
    undirectedGraph->AddVertex();
    }

  vtkSmartPointer<vtkEdgeListIterator> edgeIterator = vtkSmartPointer<vtkEdgeListIterator>::New();
  tree->GetEdges(edgeIterator);
  while(edgeIterator->HasNext())
  {
    vtkEdgeType edge = edgeIterator->Next();
    //std::cout << "Source: " << Edge.Source << " Target: " << Edge.Target << std::endl;
    undirectedGraph->AddEdge(edge.Source, edge.Target);
  }

  undirectedGraph->SetPoints(tree->GetPoints());
}
