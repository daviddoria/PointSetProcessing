#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkXMLPolyDataReader.h"

#include "vtkPointSetOutlierRemoval.h"

int main (int argc, char *argv[])
{
  if(argc != 3)
    {
    std::cout << "Required arguments: InputFilename OutputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string InputFilename = argv[1];
  std::string OutputFilename = argv[2];

  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(InputFilename.c_str());
  reader->Update();

  // Remove outliers
  vtkSmartPointer<vtkPointSetOutlierRemoval> outlierRemoval = vtkSmartPointer<vtkPointSetOutlierRemoval>::New();
  outlierRemoval->SetInputData(reader->GetOutput());
  outlierRemoval->SetPercentToRemove(.01); //remove 1% of the points
  outlierRemoval->Update();

  vtkPolyData* outputPolydata = outlierRemoval->GetOutput();

  vtkSmartPointer<vtkXMLPolyDataWriter> outputWriter = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  outputWriter->SetFileName(OutputFilename.c_str());
  outputWriter->SetInputData(outputPolydata);
  outputWriter->Write();

  return EXIT_SUCCESS;
}
