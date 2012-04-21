#include <vtkPolyData.h>
#include <vtkGraph.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkGraphReader.h>

#include <limits>

#include "vtkEuclideanMinimumSpanningTree.h"

template<class A>
bool fuzzyCompare(A a, A b)
{
  return fabs(a - b) < std::numeric_limits<A>::epsilon();
}

int main (int argc, char *argv[])
{
  // Verify command line arguments
  if(argc != 3)
    {
    std::cout << "Required arguments: InputFilename GroundTruthFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFileName = argv[1];
  std::string groundTruthFileName = argv[2];

  // Read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> inputReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  inputReader->SetFileName(inputFileName.c_str());
  inputReader->Update();

  vtkPolyData* inputPolyData = inputReader->GetOutput();

  // Estimate normals
  vtkSmartPointer<vtkEuclideanMinimumSpanningTree> emstFilter = vtkSmartPointer<vtkEuclideanMinimumSpanningTree>::New();
  emstFilter->SetInput(inputReader->GetOutput());
  emstFilter->Update();

  vtkGraph* emst = emstFilter->GetOutput();

  // Read the ground truth file
  vtkSmartPointer<vtkGraphReader> groundTruthReader = vtkSmartPointer<vtkGraphReader>::New();
  groundTruthReader->SetFileName(groundTruthFileName.c_str());
  groundTruthReader->Update();

  vtkGraph* groundTruth = groundTruthReader->GetOutput();

  if(groundTruth->GetNumberOfVertices() != emst->GetNumberOfVertices())
    {
    std::cout << "GroundTruth->GetNumberOfVertices() != NearestNeighborGraph->GetNumberOfVertices()" << std::endl;
    std::cout << "ground truth: " << groundTruth->GetNumberOfVertices() << std::endl;
    std::cout << "nearest neighbor graph: " << emst->GetNumberOfVertices() << std::endl;
    return EXIT_FAILURE;
    }

  if(groundTruth->GetNumberOfEdges() != emst->GetNumberOfEdges())
    {
    std::cout << "GroundTruth->GetNumberOfEdges() != NearestNeighborGraph->GetNumberOfEdges()" << std::endl;
    std::cout << "ground truth: " << groundTruth->GetNumberOfEdges() << std::endl;
    std::cout << "nearest neighbor graph: " << emst->GetNumberOfEdges() << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
