#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include "vtkPointSetCurvatureEstimation.h"

int main (int argc, char *argv[])
{
  // Verify command line arguements
  if(argc != 3)
    {
    std::cout << "Required arguments: InputFilename(vtp) OutputFilename(vtp)" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguements
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  // Read the input point set
  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  vtkPolyData* inputPolyData = reader->GetOutput();

  // Estimate the curvature
  vtkSmartPointer<vtkPointSetCurvatureEstimation> curvatureEstimationFilter = vtkSmartPointer<vtkPointSetCurvatureEstimation>::New();
  curvatureEstimationFilter->SetInputData(inputPolyData);
  curvatureEstimationFilter->Update();

  vtkPolyData* curvatureEstimate = curvatureEstimationFilter->GetOutput();

  //write the output point set with curvature annotation to a file
  vtkSmartPointer<vtkXMLPolyDataWriter> Writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  Writer->SetFileName(outputFilename.c_str());
  Writer->SetInputData(curvatureEstimate);
  Writer->Write();

  return EXIT_SUCCESS;
}
