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

  vtkSmartPointer<vtkMutableUndirectedGraph> g = vtkSmartPointer<vtkMutableUndirectedGraph>::New();

  // Create a vector to store vertex IDs
  std::vector<vtkIdType> vertices(input->GetNumberOfPoints());

  // Add a vertex for every point
  for(vtkIdType i = 0; i < input->GetNumberOfPoints(); i++)
  {
    vertices[i] = g->AddVertex();
  }

  // Create the edge weight array
  vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
  weights->SetNumberOfComponents(1);
  weights->SetName("Weights");

  // Add an edge from every point to every other point (if they do not already exist and they are not self loops)
  for(vtkIdType pointID = 0; pointID < input->GetNumberOfPoints(); pointID++)
  {
    for(vtkIdType neighborID = 0; neighborID < input->GetNumberOfPoints(); neighborID++)
      {
      // Do not create self loops
      if(pointID == neighborID)
        {
        continue;
        }

      // Check if the edge already exists
      vtkSmartPointer<vtkAdjacentVertexIterator> iterator = vtkSmartPointer<vtkAdjacentVertexIterator>::New();
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

      // Add the new edge
      if(!edgeAlreadyExists)
        {
        // Get the coordinates of the two points
        double point[3];
        input->GetPoint(pointID, point);

        double neighbor[3];
        input->GetPoint(neighborID, neighbor);

        g->AddEdge(pointID, neighborID);

        // Set the edge weight to the distance between the points
        double neighborPoint[3];
        input->GetPoint(neighborID, neighborPoint);
        double w = sqrt(vtkMath::Distance2BetweenPoints(point, neighbor));
        // std::cout << "Distance: " << w << std::endl;
        weights->InsertNextValue(w);

        }
    } // End neighbor loop
  } // End point loop

  // Add the point coordinates to the graph
  g->SetPoints(input->GetPoints());

  // Add the edge weight array to the graph
  g->GetEdgeData()->AddArray(weights);

  // Compute the minimum spanning tree
  vtkSmartPointer<vtkBoostPrimMinimumSpanningTree> mstFilter =
    vtkSmartPointer<vtkBoostPrimMinimumSpanningTree>::New();
  mstFilter->SetOriginVertex(0);
  mstFilter->SetInputData(g);
  mstFilter->SetEdgeWeightArrayName("Weights");
  mstFilter->Update();

  // Set the output of the filter
  output->ShallowCopy(mstFilter->GetOutput());

  // Why doesn't MST preseve points??
  output->SetPoints(input->GetPoints());

  return 1;
}

int vtkEuclideanMinimumSpanningTree::FillInputPortInformation( int port, vtkInformation* info )
{

  // The input should be a vtkDataSet
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
