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
  return fabs(a - b) < vtkstd::numeric_limits<A>::epsilon();
}

int main (int argc, char *argv[])
{
  //verify command line arguments
  if(argc != 3)
    {
    vtkstd::cout << "Required arguments: InputFilename GroundTruthFilename" << vtkstd::endl;
    return EXIT_FAILURE;
    }
  
  //parse command line arguments
  vtkstd::string InputFilename = argv[1];
  vtkstd::string GroundTruthFilename = argv[2];
  
  //read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> InputReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  InputReader->SetFileName(InputFilename.c_str());
  InputReader->Update();
  
  vtkPolyData* InputPolyData = InputReader->GetOutput();
  
  //estimate normals
  vtkSmartPointer<vtkEuclideanMinimumSpanningTree> EMSTFilter = vtkSmartPointer<vtkEuclideanMinimumSpanningTree>::New();
  EMSTFilter->SetInput(InputReader->GetOutput());
  EMSTFilter->Update();
  
  vtkGraph* EMST = EMSTFilter->GetOutput();

  //read the ground truth file
  vtkSmartPointer<vtkGraphReader> GroundTruthReader = vtkSmartPointer<vtkGraphReader>::New();
  GroundTruthReader->SetFileName(GroundTruthFilename.c_str());
  GroundTruthReader->Update();

  vtkGraph* GroundTruth = GroundTruthReader->GetOutput();
  
  if(GroundTruth->GetNumberOfVertices() != EMST->GetNumberOfVertices())
    {
    vtkstd::cout << "GroundTruth->GetNumberOfVertices() != NearestNeighborGraph->GetNumberOfVertices()" << vtkstd::endl;
    vtkstd::cout << "ground truth: " << GroundTruth->GetNumberOfVertices() << vtkstd::endl;
    vtkstd::cout << "nearest neighbor graph: " << EMST->GetNumberOfVertices() << vtkstd::endl;
    return EXIT_FAILURE;
    }

  if(GroundTruth->GetNumberOfEdges() != EMST->GetNumberOfEdges())
    {
    vtkstd::cout << "GroundTruth->GetNumberOfEdges() != NearestNeighborGraph->GetNumberOfEdges()" << vtkstd::endl;
    vtkstd::cout << "ground truth: " << GroundTruth->GetNumberOfEdges() << vtkstd::endl;
    vtkstd::cout << "nearest neighbor graph: " << EMST->GetNumberOfEdges() << vtkstd::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
