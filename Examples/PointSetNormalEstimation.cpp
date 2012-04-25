#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include "vtkPointSetNormalEstimation.h"

int main (int argc, char *argv[])
{
  // Verify command line arguments
  if(argc != 3)
    {
    std::cout << "Required arguments: InputFilename OutputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFileName = argv[1];
  std::string outputFileName = argv[2];

  // Read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(inputFileName.c_str());
  reader->Update();

  // Estimate normals
  vtkSmartPointer<vtkPointSetNormalEstimation> normalEstimation = vtkSmartPointer<vtkPointSetNormalEstimation>::New();
  normalEstimation->SetInputData(reader->GetOutput());
  normalEstimation->Update();

  // Write the output file with the estimated normals
  vtkSmartPointer<vtkXMLPolyDataWriter> Writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  Writer->SetFileName(outputFileName.c_str());
  Writer->SetInputData(normalEstimation->GetOutput());
  Writer->Write();

  return 0;
}
