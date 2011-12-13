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
  return fabs(a - b) < vtkstd::numeric_limits<A>::epsilon();
}

int main (int argc, char *argv[])
{
  //verify command line arguments
  if(argc != 3)
    {
    vtkstd::cout << "Required arguments invalid!" << vtkstd::endl;
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
  vtkSmartPointer<vtkPointSetNormalOrientation> NormalOrientation = vtkSmartPointer<vtkPointSetNormalOrientation>::New();
  NormalOrientation->SetInput(InputReader->GetOutput());
  NormalOrientation->Update();
  
  vtkPolyData* OrientedNormalsPolyData = NormalOrientation->GetOutput();

  //read the ground truth file
  vtkSmartPointer<vtkXMLPolyDataReader> GroundTruthReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  GroundTruthReader->SetFileName(GroundTruthFilename.c_str());
  GroundTruthReader->Update();

  vtkPolyData* GroundTruthPolyData = GroundTruthReader->GetOutput();
  
  if(GroundTruthPolyData->GetNumberOfPoints() != OrientedNormalsPolyData->GetNumberOfPoints())
    {
    vtkstd::cout << "Number of points do not match!" << vtkstd::endl;
    vtkstd::cout << "Ground truth points: " << GroundTruthPolyData->GetNumberOfPoints() << vtkstd::endl;
    vtkstd::cout << "Input points: " << OrientedNormalsPolyData->GetNumberOfPoints() << vtkstd::endl;
    return EXIT_FAILURE;
    }

  //get both sets of normals
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
        vtkstd::cout << "!fuzzyCompare(gt[p], est[p]))" << vtkstd::endl;
        vtkstd::cout << "gt: " << gt[p] << vtkstd::endl;
        vtkstd::cout << "est: " << est[p] << vtkstd::endl;
        return EXIT_FAILURE;
      }
    }
  }  

  return EXIT_SUCCESS;
}
