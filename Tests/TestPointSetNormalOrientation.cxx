#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include <vtkstd/limits>

#include "vtkPointSetNormalOrientation.h"

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
    std::cout << "Required arguments invalid!" << std::endl;
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

  // Estimate normals
  vtkSmartPointer<vtkPointSetNormalOrientation> NormalOrientation = vtkSmartPointer<vtkPointSetNormalOrientation>::New();
  NormalOrientation->SetInput(InputReader->GetOutput());
  NormalOrientation->Update();
  
  vtkPolyData* OrientedNormalsPolyData = NormalOrientation->GetOutput();

  // Read the ground truth file
  vtkSmartPointer<vtkXMLPolyDataReader> GroundTruthReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  GroundTruthReader->SetFileName(GroundTruthFilename.c_str());
  GroundTruthReader->Update();

  vtkPolyData* GroundTruthPolyData = GroundTruthReader->GetOutput();
  
  if(GroundTruthPolyData->GetNumberOfPoints() != OrientedNormalsPolyData->GetNumberOfPoints())
    {
    std::cout << "Number of points do not match!" << std::endl;
    std::cout << "Ground truth points: " << GroundTruthPolyData->GetNumberOfPoints() << std::endl;
    std::cout << "Input points: " << OrientedNormalsPolyData->GetNumberOfPoints() << std::endl;
    return EXIT_FAILURE;
    }

  // Get both sets of normals
  vtkSmartPointer<vtkDoubleArray> GroundTruthNormals = 
    vtkDoubleArray::SafeDownCast(GroundTruthPolyData->GetPointData()->GetNormals());
  vtkSmartPointer<vtkDoubleArray> OrientedNormals =
    vtkDoubleArray::SafeDownCast(OrientedNormalsPolyData->GetPointData()->GetNormals());

  for(unsigned int i = 0; i < GroundTruthPolyData->GetNumberOfPoints(); i++)
  {
    double gt[3];
    double est[3];
    OrientedNormals->GetTuple(i, est);
    GroundTruthNormals->GetTuple(i, gt);
  
    for(unsigned int p = 0; p < 3; p++)
    {
      if(!fuzzyCompare(gt[p], est[p]))
      {
        std::cout << "!fuzzyCompare(gt[p], est[p]))" << std::endl;
        std::cout << "gt: " << gt[p] << std::endl;
        std::cout << "est: " << est[p] << std::endl;
        return EXIT_FAILURE;
      }
    }
  }  

  return EXIT_SUCCESS;
}
