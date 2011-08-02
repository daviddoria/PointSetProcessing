#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"

#include "vtkPointSetNormalOrientation.h"

int main (int argc, char *argv[])
{
  //verify command line arguments
  if(argc != 3)
    {
    vtkstd::cout << "Required arguments: InputFilename OutputFilename" << vtkstd::endl;
    exit(-1);
    }
  
  //parse command line arguments
  vtkstd::string InputFilename = argv[1];
  vtkstd::string OutputFilename = argv[2];
  
  //read input file
  vtkSmartPointer<vtkXMLPolyDataReader> Reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  Reader->SetFileName(InputFilename.c_str());
  Reader->Update();
  
  //perform normal orientation
  vtkSmartPointer<vtkPointSetNormalOrientation> NormalOrientationFilter = vtkSmartPointer<vtkPointSetNormalOrientation>::New();
  NormalOrientationFilter->SetInput(Reader->GetOutput());
  NormalOrientationFilter->SetKNearestNeighbors(10);
  NormalOrientationFilter->Update();
  
  //write the new normals to a file
  vtkSmartPointer<vtkXMLPolyDataWriter> Writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  Writer->SetFileName(OutputFilename.c_str());
  Writer->SetInput(NormalOrientationFilter->GetOutput());
  Writer->Write();
    
  return 0;
}
