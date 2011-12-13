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
  
  //estimate normals
  vtkSmartPointer<vtkPointSetOutlierRemoval> OutlierRemoval = vtkSmartPointer<vtkPointSetOutlierRemoval>::New();
  OutlierRemoval->SetInput(InputReader->GetOutput());
  OutlierRemoval->SetPercentToRemove(.01); //remove 1% of the points
  OutlierRemoval->Update();
  
  vtkPolyData* OutliersRemoved = OutlierRemoval->GetOutput();

  //read the ground truth file
  vtkSmartPointer<vtkXMLPolyDataReader> GroundTruthReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  GroundTruthReader->SetFileName(GroundTruthFilename.c_str());
  GroundTruthReader->Update();

  vtkPolyData* GroundTruth = GroundTruthReader->GetOutput();
  if(GroundTruth->GetNumberOfPoints() != OutliersRemoved->GetNumberOfPoints())
    {
    vtkstd::cout << "GroundTruth->GetNumberOfPoints() != OutliersRemoved->GetNumberOfPoints()" << vtkstd::endl;
    vtkstd::cout << "GroundTruth->GetNumberOfPoints() = " << GroundTruth->GetNumberOfPoints() << vtkstd::endl;
    vtkstd::cout << "OutliersRemoved->GetNumberOfPoints() = " << OutliersRemoved->GetNumberOfPoints() << vtkstd::endl;
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
        vtkstd::cout << "!fuzzyCompare(gt[p], n[p])" << vtkstd::endl;
        return EXIT_FAILURE;
        }
      }
    }  

  return EXIT_SUCCESS;
}
