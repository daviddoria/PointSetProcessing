#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkTree.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkGraphToPolyData.h>
#include <vtkGraphWriter.h>
#include <vtkGraphReader.h>

#include "vtkRiemannianGraphFilter.h"

int main (int argc, char *argv[])
{
  if(argc != 4)
    {
    vtkstd::cout << "Required arguments: InputFile(vtp) OutputFile(graph) OutputFile(vtp)" << vtkstd::endl;
    exit(-1);
    }
    
  vtkstd::string inputFilename = argv[1];
  vtkstd::string outputGraphFilename = argv[2];
  vtkstd::string outputPolyDataFilename = argv[3];
    
  vtkSmartPointer<vtkXMLPolyDataReader> inputReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  inputReader->SetFileName(inputFilename.c_str());
  inputReader->Update();
  
  /*
  vtkSmartPointer<vtkGraphReader> inputReader = vtkSmartPointer<vtkGraphReader>::New();
  inputReader->SetFileName(inputFilename.c_str());
  inputReader->Update();
  vtkTree* inputTree = vtkTree::SafeDownCast(inputReader->GetOutput());
  */
  
  vtkPolyData* inputPolyData = inputReader->GetOutput();
  
  vtkSmartPointer<vtkRiemannianGraphFilter> riemannianGraphFilter = vtkSmartPointer<vtkRiemannianGraphFilter>::New();
  riemannianGraphFilter->SetkNeighbors(3);
  riemannianGraphFilter->SetInput(inputPolyData);
  riemannianGraphFilter->Update();
  
  vtkGraph* riemannianGraph = riemannianGraphFilter->GetOutput();
  
  //write the graph to a file
  vtkSmartPointer<vtkGraphWriter> graphWriter = vtkSmartPointer<vtkGraphWriter>::New();
  graphWriter->SetInput(riemannianGraph);
  graphWriter->SetFileName(outputGraphFilename.c_str());
  graphWriter->Write();
  
  //convert the graph to a polydata
  vtkSmartPointer<vtkGraphToPolyData> graphToPolyDataFilter = vtkSmartPointer<vtkGraphToPolyData>::New();
  graphToPolyDataFilter->SetInput(riemannianGraph);
  graphToPolyDataFilter->Update();
  
  //write the result to a file
  vtkSmartPointer<vtkXMLPolyDataWriter> polyDataWriter = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  polyDataWriter->SetInput(graphToPolyDataFilter->GetOutput());
  polyDataWriter->SetFileName(outputPolyDataFilename.c_str());
  polyDataWriter->Write();
  
  return 0;
}
