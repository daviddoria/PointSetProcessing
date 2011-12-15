#include <vtkPolyData.h>
#include <vtkGraph.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkGraphReader.h>

#include <vtkstd/limits>

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
  std::string InputFilename = argv[1];
  std::string GroundTruthFilename = argv[2];
  
  // Read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> InputReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  InputReader->SetFileName(InputFilename.c_str());
  InputReader->Update();
  
  vtkPolyData* InputPolyData = InputReader->GetOutput();
  
  // Estimate normals
  vtkSmartPointer<vtkEuclideanMinimumSpanningTree> EMSTFilter = vtkSmartPointer<vtkEuclideanMinimumSpanningTree>::New();
  EMSTFilter->SetInput(InputReader->GetOutput());
  EMSTFilter->Update();
  
  vtkGraph* EMST = EMSTFilter->GetOutput();

  // Read the ground truth file
  vtkSmartPointer<vtkGraphReader> GroundTruthReader = vtkSmartPointer<vtkGraphReader>::New();
  GroundTruthReader->SetFileName(GroundTruthFilename.c_str());
  GroundTruthReader->Update();

  vtkGraph* GroundTruth = GroundTruthReader->GetOutput();
  
  if(GroundTruth->GetNumberOfVertices() != EMST->GetNumberOfVertices())
    {
    std::cout << "GroundTruth->GetNumberOfVertices() != NearestNeighborGraph->GetNumberOfVertices()" << std::endl;
    std::cout << "ground truth: " << GroundTruth->GetNumberOfVertices() << std::endl;
    std::cout << "nearest neighbor graph: " << EMST->GetNumberOfVertices() << std::endl;
    return EXIT_FAILURE;
    }

  if(GroundTruth->GetNumberOfEdges() != EMST->GetNumberOfEdges())
    {
    std::cout << "GroundTruth->GetNumberOfEdges() != NearestNeighborGraph->GetNumberOfEdges()" << std::endl;
    std::cout << "ground truth: " << GroundTruth->GetNumberOfEdges() << std::endl;
    std::cout << "nearest neighbor graph: " << EMST->GetNumberOfEdges() << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
