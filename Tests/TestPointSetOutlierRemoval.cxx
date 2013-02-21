#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include <limits>

#include "vtkPointSetOutlierRemoval.h"

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

  // Estimate normals
  vtkSmartPointer<vtkPointSetOutlierRemoval> outlierRemoval = vtkSmartPointer<vtkPointSetOutlierRemoval>::New();
  outlierRemoval->SetInputConnection(inputReader->GetOutputPort());
  outlierRemoval->SetPercentToRemove(.01); //remove 1% of the points
  outlierRemoval->Update();

  vtkPolyData* outliersRemoved = outlierRemoval->GetOutput();

  // Read the ground truth file
  vtkSmartPointer<vtkXMLPolyDataReader> groundTruthReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  groundTruthReader->SetFileName(groundTruthFileName.c_str());
  groundTruthReader->Update();

  vtkPolyData* groundTruth = groundTruthReader->GetOutput();
  if(groundTruth->GetNumberOfPoints() != outliersRemoved->GetNumberOfPoints())
    {
    std::cout << "GroundTruth->GetNumberOfPoints() != OutliersRemoved->GetNumberOfPoints()" << std::endl;
    std::cout << "GroundTruth->GetNumberOfPoints() = " << groundTruth->GetNumberOfPoints() << std::endl;
    std::cout << "OutliersRemoved->GetNumberOfPoints() = " << outliersRemoved->GetNumberOfPoints() << std::endl;
    return EXIT_FAILURE;
    }

  for(vtkIdType i = 0; i < groundTruth->GetNumberOfPoints(); i++)
    {
    double gt[3];
    groundTruth->GetPoints()->GetPoint(i,gt);
    double n[3];
    outliersRemoved->GetPoints()->GetPoint(i,n);
    for(unsigned int p = 0; p < 3; p++)
      {
      if(!fuzzyCompare(gt[p], n[p]))
        {
        std::cout << "!fuzzyCompare(gt[p], n[p])" << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  return EXIT_SUCCESS;
}
