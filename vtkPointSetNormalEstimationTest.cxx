#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#if VTK_MAJOR_VERSION>5 || (VTK_MAJOR_VERSION==5 && VTK_MINOR_VERSION>4)
#include <vtkstd/limits>
#else
#include <limits>
#endif

#include "vtkPointSetNormalEstimation.h"

template<class A>
bool fuzzyCompare(A a, A b) 
{
  return fabs(a - b) < vtkstd::numeric_limits<A>::epsilon();
}

int main (int argc, char *argv[])
{
  //verify command line arguments
  if(argc != 3)
    {
    cout << "Required arguments not specified!" << endl;
    cout << "Required arguments: InputFilename GroundTruthFilename" << endl;
    return EXIT_FAILURE;
    }
  
  //parse command line arguments
  vtkstd::string inputFilename = argv[1];
  vtkstd::string groundTruthFilename = argv[2];
  
  //read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> inputReader = 
      vtkSmartPointer<vtkXMLPolyDataReader>::New();
  inputReader->SetFileName(inputFilename.c_str());
  inputReader->Update();
  
  //estimate normals
  vtkSmartPointer<vtkPointSetNormalEstimation> normalEstimation = 
      vtkSmartPointer<vtkPointSetNormalEstimation>::New();
  normalEstimation->SetInput(inputReader->GetOutput());
  normalEstimation->SetkNeighbors(5);
  normalEstimation->Update();
  
  vtkPolyData* estimatedNormalsPolyData = normalEstimation->GetOutput();

  /*
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetInput(EstimatedNormalsPolyData);
  writer->SetFileName("output.vtp");
  writer->Write();
  */
  
  //read the ground truth file
  vtkSmartPointer<vtkXMLPolyDataReader> groundTruthReader = 
      vtkSmartPointer<vtkXMLPolyDataReader>::New();
  groundTruthReader->SetFileName(groundTruthFilename.c_str());
  groundTruthReader->Update();

  vtkPolyData* groundTruthPolyData = groundTruthReader->GetOutput();
  
  //check that the number of points match
  if(groundTruthPolyData->GetNumberOfPoints() != estimatedNormalsPolyData->GetNumberOfPoints())
    {
    cout << "Number of points does not match!" << endl;
    cout << "Estimated points: " << estimatedNormalsPolyData->GetNumberOfPoints() << endl;
    cout << "Ground truth points: " << groundTruthPolyData->GetNumberOfPoints() << endl;
    return EXIT_FAILURE;
    }

  //get both sets of normals
    vtkSmartPointer<vtkDoubleArray> groundTruthNormals = 
            vtkDoubleArray::SafeDownCast(groundTruthPolyData->GetPointData()->GetNormals());
  vtkSmartPointer<vtkDoubleArray> estimatedNormals =
          vtkDoubleArray::SafeDownCast(estimatedNormalsPolyData->GetPointData()->GetNormals());

  for(unsigned int i = 0; i < groundTruthPolyData->GetNumberOfPoints(); i++)
    {
    double gt[3];
    double est[3];
    estimatedNormals->GetTuple(i, est);
    groundTruthNormals->GetTuple(i, gt);
    
    for(unsigned int p = 0; p < 3; p++)
      {
      if(!fuzzyCompare(gt[p], est[p]))
        {
        cout << "!fuzzyCompare(gt[p], est[p]))" << endl;
        cout << "gt: " << gt[p] << endl;
        cout << "est: " << est[p] << endl;
        return EXIT_FAILURE;
        }
      }
    }  

    
  return EXIT_SUCCESS;
}
