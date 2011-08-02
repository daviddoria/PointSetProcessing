#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include "vtkPointSetNormalEstimation.h"

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
  
  //read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> Reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  Reader->SetFileName(InputFilename.c_str());
  Reader->Update();
  
  //estimate normals
  vtkSmartPointer<vtkPointSetNormalEstimation> NormalEstimation = vtkSmartPointer<vtkPointSetNormalEstimation>::New();
  NormalEstimation->SetInput(Reader->GetOutput());
  NormalEstimation->Update();
  
  //write the output file with the estimated normals
  vtkSmartPointer<vtkXMLPolyDataWriter> Writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  Writer->SetFileName(OutputFilename.c_str());
  Writer->SetInput(NormalEstimation->GetOutput());
  Writer->Write();
  
  return 0;
}
