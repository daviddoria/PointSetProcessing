#include <vtkPolyData.h>
#include <vtkTree.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkGraphToPolyData.h>
#include <vtkGraphWriter.h>

#include "vtkEuclideanMinimumSpanningTree.h"

int main (int argc, char *argv[])
{
  if(argc != 4)
    {
    vtkstd::cout << "Required arguments: InputFile(vtp) OutputFile(graph) OutputFile(vtp)" << vtkstd::endl;
    exit(-1);
    }
    
  vtkstd::string InputFilename = argv[1];
  vtkstd::string OutputGraphFilename = argv[2];
  vtkstd::string OutputPolyDataFilename = argv[3];
    
  vtkSmartPointer<vtkXMLPolyDataReader> Reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  Reader->SetFileName(InputFilename.c_str());
  Reader->Update();
  
  vtkPolyData* InputPolydata = Reader->GetOutput();
  
  vtkSmartPointer<vtkEuclideanMinimumSpanningTree> EMSTFilter = vtkSmartPointer<vtkEuclideanMinimumSpanningTree>::New();
  EMSTFilter->SetInput(InputPolydata);
  EMSTFilter->Update();
  
  vtkTree* EMST = EMSTFilter->GetOutput();
  
    //write the graph to a file
  vtkSmartPointer<vtkGraphWriter> GraphWriter = vtkSmartPointer<vtkGraphWriter>::New();
  GraphWriter->SetInput(EMST);
  GraphWriter->SetFileName(OutputGraphFilename.c_str());
  GraphWriter->Write();
  
  //convert the graph to a polydata
  vtkSmartPointer<vtkGraphToPolyData> GraphToPolyData = vtkSmartPointer<vtkGraphToPolyData>::New();
  GraphToPolyData->SetInput(EMST);
  GraphToPolyData->Update();
  
  //write the result to a file
  vtkSmartPointer<vtkXMLPolyDataWriter> PolyDataWriter = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  PolyDataWriter->SetInput(GraphToPolyData->GetOutput());
  PolyDataWriter->SetFileName(OutputPolyDataFilename.c_str());
  PolyDataWriter->Write();
  
  return 0;
}
