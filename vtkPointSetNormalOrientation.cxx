// STL
#include <queue>
#include <limits>

// VTK
#include <vtkAdjacentVertexIterator.h>
#include <vtkBoostConnectedComponents.h>
#include <vtkBoostPrimMinimumSpanningTree.h>
#include <vtkCellArray.h>
#include <vtkCommand.h>
#include <vtkDoubleArray.h>
#include <vtkEdgeListIterator.h>
#include <vtkGraphToPolyData.h>
#include <vtkKdTreePointLocator.h>
#include <vtkObjectFactory.h> //for new() macro
#include <vtkIdList.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkIntArray.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkTree.h>
#include <vtkMath.h>
#include <vtkMutableUndirectedGraph.h>
#include <vtkTreeDFSIterator.h>
#include <vtkTreeDFSIterator.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataWriter.h>

// Custom
#include "vtkPointSetNormalOrientation.h"
#include "vtkRiemannianGraphFilter.h"
#include "vtkKNNGraphFilter.h"

vtkStandardNewMacro(vtkPointSetNormalOrientation);

vtkPointSetNormalOrientation::vtkPointSetNormalOrientation()
{
  this->GraphFilterType = RIEMANN_GRAPH;
  this->KNearestNeighbors = 5;
  this->IterateEvent = vtkCommand::UserEvent + 1;
  this->IterateEvent = vtkCommand::UserEvent + 2;
}


int vtkPointSetNormalOrientation::RequestData(vtkInformation *vtkNotUsed(request),
                                             vtkInformationVector **inputVector,
                                             vtkInformationVector *outputVector)
{
  // This function calls the scanners input and output to allow it to
  // work in the vtk algorithm pipeline

  // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkUndirectedGraph* graph = vtkSmartPointer<vtkUndirectedGraph>::New();
  vtkSmartPointer<vtkKNNGraphFilter> KNNGraphFilter = vtkSmartPointer<vtkKNNGraphFilter>::New();
  vtkSmartPointer<vtkRiemannianGraphFilter> riemannianGraphFilter = vtkSmartPointer<vtkRiemannianGraphFilter>::New();
  if (this->GraphFilterType == KNN_GRAPH)
  {    
    KNNGraphFilter->SetInputData(input);
    KNNGraphFilter->SetKNeighbors(this->KNearestNeighbors);
    KNNGraphFilter->Update();
    graph = vtkUndirectedGraph::SafeDownCast(KNNGraphFilter->GetOutput());
  }
  else if (this->GraphFilterType == RIEMANN_GRAPH)
  {    
    riemannianGraphFilter->SetInputData(input);
    riemannianGraphFilter->Update();
    graph = vtkUndirectedGraph::SafeDownCast(riemannianGraphFilter->GetOutput());
  }
  else
  {
    std::cerr << "Wrong GraphFilterType! Should be either 0 (RIEMANN_GRAPH) or 1 (KNN_GRAPH)." << std::endl;
    return 0;
  }

  std::string message = "Constructed connectivity graph.";
  this->InvokeEvent(this->ProgressEvent, &message);

  // Find the top point to act as the seed for the normal propagation
  unsigned int maxZId = FindMaxZId(input);
  std::cout << "MaxZId: " << maxZId << std::endl;

  // Create the edge weight array
  vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
  weights->SetNumberOfComponents(1);
  weights->SetName("Weights");

  // Reweight edges
  vtkSmartPointer<vtkEdgeListIterator> edgeListIterator = vtkSmartPointer<vtkEdgeListIterator>::New();
  graph->GetEdges(edgeListIterator);
  while(edgeListIterator->HasNext())
    {
      vtkEdgeType Edge = edgeListIterator->Next();
      double sourceNormal[3];
      double targetNormal[3];
      input->GetPointData()->GetNormals()->GetTuple(Edge.Source, sourceNormal);
      input->GetPointData()->GetNormals()->GetTuple(Edge.Target, targetNormal);
      double w = 1.0 - fabs(vtkMath::Dot(sourceNormal, targetNormal));
      if (w < 0)
      {
        w = 0;
      }
      weights->InsertNextValue(w);
    }

  // Add the edge weight array to the graph
  graph->GetEdgeData()->AddArray(weights);

  // std::cout << "Number of Weights: " <<
  // vtkDoubleArray::SafeDownCast(PointGraph->GetEdgeData()->GetArray("Weights"))->GetNumberOfTuples() << std::endl;

  message = "Reweighted edges.";
  this->InvokeEvent(this->ProgressEvent, &message);
  
  // Find the minimum spanning tree on the Riemannian graph, starting from the highest point
  vtkSmartPointer<vtkBoostPrimMinimumSpanningTree> mstFilter = vtkSmartPointer<vtkBoostPrimMinimumSpanningTree>::New();
  mstFilter->SetOriginVertex(maxZId);
  mstFilter->SetInputData(graph);
  mstFilter->SetEdgeWeightArrayName("Weights");
  mstFilter->Update();

  // Get the output tree
  vtkSmartPointer<vtkTree> mst = vtkSmartPointer<vtkTree>::New();
  mst->ShallowCopy(mstFilter->GetOutput());

  message = "Found MST.";
  this->InvokeEvent(this->ProgressEvent, &message);
  /*
  vtkSmartPointer<vtkGraphLayoutView> MSTView = vtkSmartPointer<vtkGraphLayoutView>::New();
  MSTView->AddRepresentationFromInput(MinimumSpanningTree);
  MSTView->SetLayoutStrategyToTree();
  MSTView->ResetCamera();
  MSTView->Render();
  MSTView->GetInteractor()->Start();
  */

  // Traverse the minimum spanning tree to propagate the normal direction
  vtkSmartPointer<vtkTreeDFSIterator> dfsIterator = vtkSmartPointer<vtkTreeDFSIterator>::New();
  dfsIterator->SetStartVertex(maxZId);
  dfsIterator->SetTree(mst);

  vtkDataArray* oldNormals = input->GetPointData()->GetNormals();

  vtkSmartPointer<vtkDoubleArray> newNormals = vtkSmartPointer<vtkDoubleArray>::New();
  newNormals->SetName("Normals");
  newNormals->SetNumberOfComponents(3); //3d normals (ie x,y,z)
  newNormals->SetNumberOfTuples(input->GetNumberOfPoints());

  // Traverse the tree (depth first) and flip normals if necessary.
  //The first normal should be facing (out). Since we used the point with the max z coordinate as the
  // root of this traversal tree, we should check it's normal against the "up" (+z) vector.
  double lastNormal[3] = {0.0,0.0,1.0};
  unsigned int nodesVisited = 0;
  unsigned int flippedNormals = 0;
  while(dfsIterator->HasNext())
    {
    nodesVisited++;
    vtkIdType nextVertex = dfsIterator->Next();
    //std::cout << "Next vertex: " << NextVertex << std::endl;
    double oldNormal[3];

    oldNormals->GetTuple(nextVertex, oldNormal);
    //std::cout << "Old normal: " << OldNormal[0] << " " << OldNormal[1] << " " << OldNormal[2] << " ";
    if(vtkMath::Dot(oldNormal, lastNormal) < 0.0) //the normal is facing the "wrong" way
      {
      // Flip the normal
      vtkMath::MultiplyScalar(oldNormal, -1.0);
      flippedNormals++;
      //std::cout << "New normal: " << OldNormal[0] << " " << OldNormal[1] << " " << OldNormal[2] << " ";
      }

    // std::cout << std::endl;
    newNormals->SetTuple(nextVertex, oldNormal);

    lastNormal[0] = oldNormal[0];
    lastNormal[1] = oldNormal[1];
    lastNormal[2] = oldNormal[2];
    }

  //std::cout << "Nodes visited: " << NodesVisited << std::endl;
  //std::cout << "Total nodes: " << MinimumSpanningTree->GetNumberOfVertices() << std::endl;
  std::cout << "Flipped normals: " << flippedNormals << std::endl;

  output->ShallowCopy(input);
  output->GetPointData()->SetNormals(newNormals);
  /*
  vtkSmartPointer<vtkPolyData> OutputPolydata = vtkSmartPointer<vtkPolyData>::New();
  OutputPolydata->ShallowCopy(input);

  //assign the new normals
  OutputPolydata->GetPointData()->SetNormals(NewNormals);

  //set the output of the filter
  output->ShallowCopy(OutputPolydata);
  */
  return 1;
}

////////// External Operators /////////////

void vtkPointSetNormalOrientation::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "K Nearest Neighbors: " << this->KNearestNeighbors << std::endl;
}

unsigned int FindMaxZId(vtkPolyData* input)
{
  unsigned int MaxZId = 0;

  double MaxZ = -1.0 * std::numeric_limits<double>::infinity();

  // Find the highest point
  for(vtkIdType i = 0; i < input->GetNumberOfPoints(); i++)
    {
    double p[3];
    input->GetPoint(i,p);
    if(p[2] > MaxZ)
      {
      MaxZId = i;
      MaxZ = p[2];
      }
    }

  return MaxZId;
}
