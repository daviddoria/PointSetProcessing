#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkMath.h>
#include <vtkGraph.h>
#include <vtkTree.h>
#include <vtkMutableUndirectedGraph.h>
#include <vtkAdjacentVertexIterator.h>
#include <vtkBoostPrimMinimumSpanningTree.h>

#include <vtkExecutive.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h> //for new() macro

#include "vtkEuclideanMinimumSpanningTree.h"

vtkStandardNewMacro(vtkEuclideanMinimumSpanningTree);

int vtkEuclideanMinimumSpanningTree::RequestDataObject(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector)
{
  vtkTree *output = 0;
  output = vtkTree::New();
    
  this->GetExecutive()->SetOutputData(0, output);
  output->Delete();

  return 1;
}

int vtkEuclideanMinimumSpanningTree::RequestData(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkTree *output = vtkTree::SafeDownCast(
                                            outInfo->Get(vtkDataObject::DATA_OBJECT()));
  
  vtkSmartPointer<vtkMutableUndirectedGraph> G = vtkSmartPointer<vtkMutableUndirectedGraph>::New();
  
  //create a vector to store vertex IDs
  vtkstd::vector<vtkIdType> Vertices(input->GetNumberOfPoints());
  
  //add a vertex for every point
  for(unsigned int i = 0; i < input->GetNumberOfPoints(); i++)
  {
    Vertices[i] = G->AddVertex();
  }
  
  //create the edge weight array
  vtkSmartPointer<vtkDoubleArray> Weights = vtkSmartPointer<vtkDoubleArray>::New();
  Weights->SetNumberOfComponents(1);
  Weights->SetName("Weights");
    
  //add an edge from every point to every other point (if they do not already exist and they are not self loops)
  for(unsigned int pointID = 0; pointID < input->GetNumberOfPoints(); pointID++)
  {
    for(unsigned int neighborID = 0; neighborID < input->GetNumberOfPoints(); neighborID++)
      {
      //do not create self loops
      if(pointID == neighborID)
        {
        continue;
        }

      //check if the edge already exists
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
        
      //add the new edge
      if(!EdgeAlreadyExists)
        {
          //get the coordinates of the two points
          double point[3];
          input->GetPoint(pointID, point);
      
          double neighbor[3];
          input->GetPoint(neighborID, neighbor);
          
          G->AddEdge(pointID, neighborID);
          
          //set the edge weight to the distance between the points
          double neighborPoint[3];
          input->GetPoint(neighborID, neighborPoint);
          double w = sqrt(vtkMath::Distance2BetweenPoints(point, neighbor));
          //vtkstd::cout << "Distance: " << w << vtkstd::endl;
          Weights->InsertNextValue(w);
          
        }
    } //end neighbor loop
  } //end point loop
  
  //add the point coordinates to the graph
  G->SetPoints(input->GetPoints());
  
  //add the edge weight array to the graph
  G->GetEdgeData()->AddArray(Weights);
  
  //compute the minimum spanning tree
  vtkSmartPointer<vtkBoostPrimMinimumSpanningTree> MSTFilter = vtkSmartPointer<vtkBoostPrimMinimumSpanningTree>::New();
  MSTFilter->SetOriginVertex(0);
  MSTFilter->SetInput(G);
  MSTFilter->SetEdgeWeightArrayName("Weights");
  MSTFilter->Update();
  
  //set the output of the filter
  output->ShallowCopy(MSTFilter->GetOutput());
  
  //why doesn't MST preseve points??
  output->SetPoints(input->GetPoints());
  
  return 1;
}

int vtkEuclideanMinimumSpanningTree::FillInputPortInformation( int port, vtkInformation* info )
{

  //The input should be a vtkDataSet
  if (port == 0)
  {
    info->Set( vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet" );
    return 1;
  }

  return 0;
}

void vtkEuclideanMinimumSpanningTree::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
