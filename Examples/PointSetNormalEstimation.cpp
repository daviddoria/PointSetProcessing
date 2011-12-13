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
  std::string InputFilename = argv[1];
  std::string OutputFilename = argv[2];
  
  // Read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> Reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  Reader->SetFileName(InputFilename.c_str());
  Reader->Update();
  
  // Estimate normals
  vtkSmartPointer<vtkPointSetNormalEstimation> NormalEstimation = vtkSmartPointer<vtkPointSetNormalEstimation>::New();
  NormalEstimation->SetInput(Reader->GetOutput());
  NormalEstimation->Update();
  
  // Write the output file with the estimated normals
  vtkSmartPointer<vtkXMLPolyDataWriter> Writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  Writer->SetFileName(OutputFilename.c_str());
  Writer->SetInput(NormalEstimation->GetOutput());
  Writer->Write();
  
  return 0;
}
