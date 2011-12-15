#include <queue>
#include <limits>

#include <vtkObjectFactory.h> //for new() macro
#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkTree.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPolyData.h>
#include <vtkMath.h>
#include <vtkKdTreePointLocator.h>
#include <vtkIntArray.h>

#include <vtkMutableUndirectedGraph.h>
#include <vtkBoostPrimMinimumSpanningTree.h>
#include <vtkTreeDFSIterator.h>
#include <vtkAdjacentVertexIterator.h>
#include <vtkBoostPrimMinimumSpanningTree.h>
#include <vtkTreeDFSIterator.h>
#include <vtkEdgeListIterator.h>

#include <vtkXMLPolyDataWriter.h>
#include <vtkBoostConnectedComponents.h>
#include <vtkGraphToPolyData.h>
#include <vtkXMLPolyDataWriter.h>

//#include <vtkGraphLayoutView.h>
//#include <vtkRenderWindowInteractor.h>

#include "vtkPointSetNormalOrientation.h"
#include "vtkRiemannianGraphFilter.h"

vtkStandardNewMacro(vtkPointSetNormalOrientation);

vtkPointSetNormalOrientation::vtkPointSetNormalOrientation()
{
  this->KNearestNeighbors = 5;
}


int vtkPointSetNormalOrientation::RequestData(vtkInformation *vtkNotUsed(request),
                                             vtkInformationVector **inputVector,
                                             vtkInformationVector *outputVector)
{
  //This function calls the scanners input and output to allow it to 
  //work in the vtk algorithm pipeline
  
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));

  /*
  vtkSmartPointer<vtkNearestNeighborGraph> NearestNeighborGraphFilter = vtkSmartPointer<vtkNearestNeighborGraph>::New();
  NearestNeighborGraphFilter->SetInput(input);
  NearestNeighborGraphFilter->SetkNeighbors(this->KNearestNeighbors);
  NearestNeighborGraphFilter->Update();
  */
  vtkSmartPointer<vtkRiemannianGraphFilter> RiemannianGraphFilter = vtkSmartPointer<vtkRiemannianGraphFilter>::New();
  RiemannianGraphFilter->SetInput(input);
  RiemannianGraphFilter->Update();
  
  vtkUndirectedGraph* RiemannianGraph = vtkUndirectedGraph::SafeDownCast(RiemannianGraphFilter->GetOutput());
  

  /*
  //visualize
  vtkSmartPointer<vtkGraphLayoutView> GraphLayoutView = vtkSmartPointer<vtkGraphLayoutView>::New();
  GraphLayoutView->AddRepresentationFromInput(PointGraph);
  GraphLayoutView->SetLayoutStrategyToSimple2D();
  GraphLayoutView->ResetCamera();
  GraphLayoutView->Render();
  GraphLayoutView->GetInteractor()->Start();
  */
   
  //find the top point to act as the seed for the normal propagation
  unsigned int MaxZId = FindMaxZId(input);
  vtkstd::cout << "MaxZId: " << MaxZId << vtkstd::endl;
  
  //create the edge weight array
  vtkSmartPointer<vtkDoubleArray> Weights = vtkSmartPointer<vtkDoubleArray>::New();
  Weights->SetNumberOfComponents(1);
  Weights->SetName("Weights");
   
  //reweight edges
  vtkSmartPointer<vtkEdgeListIterator> EdgeListIterator = vtkSmartPointer<vtkEdgeListIterator>::New();
  RiemannianGraph->GetEdges(EdgeListIterator);
  while(EdgeListIterator->HasNext())
  {
    vtkEdgeType Edge = EdgeListIterator->Next();
    //std::cout << "Source: " << Edge.Source << " Target: " << Edge.Target << vtkstd::endl;
    double source[3];
    double target[3];
    RiemannianGraph->GetPoints()->GetPoint(Edge.Source, source);
    RiemannianGraph->GetPoints()->GetPoint(Edge.Target, target);
    
    //double w = vtkMath::Dot(source, target);
    double w = 1.0 - fabs(vtkMath::Dot(source, target));
    //vtkstd::cout << "w: " << w << vtkstd::endl;
    
    //naive
    //double w = 1.0;
    Weights->InsertNextValue(w);
  }
  
  //add the edge weight array to the graph
  RiemannianGraph->GetEdgeData()->AddArray(Weights);
  
  //vtkstd::cout << "Number of Weights: " << vtkDoubleArray::SafeDownCast(PointGraph->GetEdgeData()->GetArray("Weights"))->GetNumberOfTuples() << vtkstd::endl;
  
  //find the minimum spanning tree on the Riemannian graph, starting from the highest point
  vtkSmartPointer<vtkBoostPrimMinimumSpanningTree> MSTFilter = vtkSmartPointer<vtkBoostPrimMinimumSpanningTree>::New();
  MSTFilter->SetOriginVertex(MaxZId);
  MSTFilter->SetInput(RiemannianGraph);
  MSTFilter->SetEdgeWeightArrayName("Weights");
  MSTFilter->Update();
  
  //get the output tree
  vtkSmartPointer<vtkTree> MST = vtkSmartPointer<vtkTree>::New();
  MST->ShallowCopy(MSTFilter->GetOutput());
  
/*
  vtkSmartPointer<vtkGraphLayoutView> MSTView = vtkSmartPointer<vtkGraphLayoutView>::New();
  MSTView->AddRepresentationFromInput(MinimumSpanningTree);
  MSTView->SetLayoutStrategyToTree();
  MSTView->ResetCamera();
  MSTView->Render();
  MSTView->GetInteractor()->Start();
  */

  //traverse the minimum spanning tree to propagate the normal direction
  vtkSmartPointer<vtkTreeDFSIterator> DFS = vtkSmartPointer<vtkTreeDFSIterator>::New();
  DFS->SetStartVertex(MaxZId);
  DFS->SetTree(MST);
  
  vtkDoubleArray* OldNormals = vtkDoubleArray::SafeDownCast(input->GetPointData()->GetNormals());

  vtkSmartPointer<vtkDoubleArray> NewNormals = vtkSmartPointer<vtkDoubleArray>::New();
  NewNormals->SetName("Normals");
  NewNormals->SetNumberOfComponents(3); //3d normals (ie x,y,z)
  NewNormals->SetNumberOfTuples(input->GetNumberOfPoints());
      
  //traverse the tree (depth first) and flip normals if necessary
  double LastNormal[3] = {0.0,0.0,1.0}; //The first normal should be facing (out). Since we used the point with the max z coordinate as the root of this traversal tree, we should check it's normal against the "up" (+z) vector
  unsigned int NodesVisited = 0;
  unsigned int FlippedNormals = 0;
  while(DFS->HasNext())
  {
    NodesVisited++;
    vtkIdType NextVertex = DFS->Next();
    //vtkstd::cout << "Next vertex: " << NextVertex << vtkstd::endl;
    double OldNormal[3];
    
    OldNormals->GetTuple(NextVertex, OldNormal);
    //vtkstd::cout << "Old normal: " << OldNormal[0] << " " << OldNormal[1] << " " << OldNormal[2] << " ";
    if(vtkMath::Dot(OldNormal, LastNormal) < 0.0) //the normal is facing the "wrong" way
    {
      // Flip the normal
      vtkMath::MultiplyScalar(OldNormal, -1.0);
      FlippedNormals++;
      //vtkstd::cout << "New normal: " << OldNormal[0] << " " << OldNormal[1] << " " << OldNormal[2] << " ";
    }
    
    //vtkstd::cout << vtkstd::endl;
    NewNormals->SetTuple(NextVertex, OldNormal);
    
    LastNormal[0] = OldNormal[0];
    LastNormal[1] = OldNormal[1];
    LastNormal[2] = OldNormal[2];
  }
  
  //vtkstd::cout << "Nodes visited: " << NodesVisited << vtkstd::endl;
  //vtkstd::cout << "Total nodes: " << MinimumSpanningTree->GetNumberOfVertices() << vtkstd::endl;
  vtkstd::cout << "Flipped normals: " << FlippedNormals << vtkstd::endl;
  
  output->ShallowCopy(input);
  output->GetPointData()->SetNormals(NewNormals);
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

  double MaxZ = -1.0 * vtkstd::numeric_limits<double>::infinity();

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
