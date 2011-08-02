#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include "vtkPointSetCurvatureEstimation.h"

int main (int argc, char *argv[])
{
  //verify command line arguements
  if(argc != 3)
    {
    vtkstd::cout << "Required arguments: InputFilename(vtp) OutputFilename(vtp)" << vtkstd::endl;
    exit(-1);
    }
  
  //parse command line arguements
  vtkstd::string InputFilename = argv[1];
  vtkstd::string OutputFilename = argv[2];
  
  //read the input point set
  vtkSmartPointer<vtkXMLPolyDataReader> Reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  Reader->SetFileName(InputFilename.c_str());
  Reader->Update();
  
  vtkPolyData* InputPolyData = Reader->GetOutput();
  
  //estimate the curvature
  vtkSmartPointer<vtkPointSetCurvatureEstimation> CurvatureEstimationFilter = vtkSmartPointer<vtkPointSetCurvatureEstimation>::New();
  CurvatureEstimationFilter->SetInput(InputPolyData);
  CurvatureEstimationFilter->Update();
  
  vtkPolyData* CurvatureEstimate = CurvatureEstimationFilter->GetOutput();
  
  //write the output point set with curvature annotation to a file
  vtkSmartPointer<vtkXMLPolyDataWriter> Writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  Writer->SetFileName(OutputFilename.c_str());
  Writer->SetInput(CurvatureEstimate);
  Writer->Write();
  
  return 0;
}
