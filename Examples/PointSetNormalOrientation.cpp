#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"

#include "vtkPointSetNormalOrientation.h"

int main (int argc, char *argv[])
{
  // Verify command line arguments
  if(argc != 3)
    {
    std::cout << "Required arguments: InputFilename OutputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  // Read input file
  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  // Perform normal orientation
  vtkSmartPointer<vtkPointSetNormalOrientation> normalOrientationFilter = vtkSmartPointer<vtkPointSetNormalOrientation>::New();
  normalOrientationFilter->SetInputData(reader->GetOutput());
  normalOrientationFilter->SetKNearestNeighbors(10);
  normalOrientationFilter->Update();

  // Write the new normals to a file
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(outputFilename.c_str());
  writer->SetInputData(normalOrientationFilter->GetOutput());
  writer->Write();

  return EXIT_SUCCESS;
}
