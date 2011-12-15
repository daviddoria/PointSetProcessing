#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include <vtkstd/limits>

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
    std::cout << "Required arguments: InputFilename GroundTruthFilename" << vtkstd::endl;
    return EXIT_FAILURE;
    }
  
  // Parse command line arguments
  std::string InputFilename = argv[1];
  std::string GroundTruthFilename = argv[2];
  
  // Read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> InputReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  InputReader->SetFileName(InputFilename.c_str());
  InputReader->Update();
  
  // Estimate normals
  vtkSmartPointer<vtkPointSetOutlierRemoval> OutlierRemoval = vtkSmartPointer<vtkPointSetOutlierRemoval>::New();
  OutlierRemoval->SetInput(InputReader->GetOutput());
  OutlierRemoval->SetPercentToRemove(.01); //remove 1% of the points
  OutlierRemoval->Update();
  
  vtkPolyData* OutliersRemoved = OutlierRemoval->GetOutput();

  // Read the ground truth file
  vtkSmartPointer<vtkXMLPolyDataReader> GroundTruthReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  GroundTruthReader->SetFileName(GroundTruthFilename.c_str());
  GroundTruthReader->Update();

  vtkPolyData* GroundTruth = GroundTruthReader->GetOutput();
  if(GroundTruth->GetNumberOfPoints() != OutliersRemoved->GetNumberOfPoints())
    {
    std::cout << "GroundTruth->GetNumberOfPoints() != OutliersRemoved->GetNumberOfPoints()" << std::endl;
    std::cout << "GroundTruth->GetNumberOfPoints() = " << GroundTruth->GetNumberOfPoints() << std::endl;
    std::cout << "OutliersRemoved->GetNumberOfPoints() = " << OutliersRemoved->GetNumberOfPoints() << std::endl;
    return EXIT_FAILURE;
    }

  for(unsigned int i = 0; i < GroundTruth->GetNumberOfPoints(); i++)
    {
    double gt[3];
    GroundTruth->GetPoints()->GetPoint(i,gt);
    double n[3];
    OutliersRemoved->GetPoints()->GetPoint(i,n);
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
