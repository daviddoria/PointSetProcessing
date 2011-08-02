#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkXMLPolyDataReader.h"

#include <vtkstd/string>

#include "vtkPointSetOutlierRemoval.h"

int main (int argc, char *argv[])
{
  if(argc != 3)
    {
    vtkstd::cout << "Required arguments: InputFilename OutputFilename" << vtkstd::endl;
    exit(-1);
    }
    
  vtkstd::string InputFilename = argv[1];
  vtkstd::string OutputFilename = argv[2];
  
  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(InputFilename.c_str());
  reader->Update();
  
  //remove outliers
  vtkSmartPointer<vtkPointSetOutlierRemoval> OutlierRemoval = vtkSmartPointer<vtkPointSetOutlierRemoval>::New();
  OutlierRemoval->SetInput(reader->GetOutput());
  OutlierRemoval->SetPercentToRemove(.01); //remove 1% of the points
  OutlierRemoval->Update();
  
  vtkPolyData* OutputPolydata = OutlierRemoval->GetOutput();
  
  vtkSmartPointer<vtkXMLPolyDataWriter> OutputWriter = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  OutputWriter->SetFileName(OutputFilename.c_str());
  OutputWriter->SetInput(OutputPolydata);
  OutputWriter->Write();
  
  return 0;
}
