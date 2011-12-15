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
    std::cout << "Required arguments: InputFile(vtp) OutputFile(graph) OutputFile(vtp)" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputGraphFilename = argv[2];
  std::string outputPolyDataFilename = argv[3];

  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  vtkPolyData* inputPolydata = reader->GetOutput();

  vtkSmartPointer<vtkEuclideanMinimumSpanningTree> emstFilter = vtkSmartPointer<vtkEuclideanMinimumSpanningTree>::New();
  emstFilter->SetInput(InputPolydata);
  emstFilter->Update();

  vtkTree* emst = emstFilter->GetOutput();

  // Write the graph to a file
  vtkSmartPointer<vtkGraphWriter> graphWriter = vtkSmartPointer<vtkGraphWriter>::New();
  graphWriter->SetInput(emst);
  graphWriter->SetFileName(outputGraphFilename.c_str());
  graphWriter->Write();

  // Convert the graph to a polydata
  vtkSmartPointer<vtkGraphToPolyData> GraphToPolyData = vtkSmartPointer<vtkGraphToPolyData>::New();
  GraphToPolyData->SetInput(emst);
  GraphToPolyData->Update();

  // Write the result to a file
  vtkSmartPointer<vtkXMLPolyDataWriter> polyDataWriter = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  polyDataWriter->SetInput(GraphToPolyData->GetOutput());
  polyDataWriter->SetFileName(outputPolyDataFilename.c_str());
  polyDataWriter->Write();

  return EXIT_SUCCESS;
}
