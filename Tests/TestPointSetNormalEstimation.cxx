#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include <limits>

#include "vtkPointSetNormalEstimation.h"

template<class A>
bool fuzzyCompare(A a, A b)
{
  return fabs(a - b) < std::numeric_limits<A>::epsilon();
}

int main (int argc, char *argv[])
{
  // Verify command line arguments
  if(argc != 3)
    {
    cout << "Required arguments not specified!" << endl;
    cout << "Required arguments: InputFilename GroundTruthFilename" << endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];
  std::string groundTruthFilename = argv[2];

  // Read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> inputReader =
    vtkSmartPointer<vtkXMLPolyDataReader>::New();
  inputReader->SetFileName(inputFilename.c_str());
  inputReader->Update();

  // Estimate normals
  vtkSmartPointer<vtkPointSetNormalEstimation> normalEstimation =
      vtkSmartPointer<vtkPointSetNormalEstimation>::New();
  normalEstimation->SetInputConnection(inputReader->GetOutputPort());
  normalEstimation->SetNumberOfNeighbors(5);
  normalEstimation->Update();

  vtkPolyData* estimatedNormalsPolyData = normalEstimation->GetOutput();

  /*
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetInput(EstimatedNormalsPolyData);
  writer->SetFileName("output.vtp");
  writer->Write();
  */

  // Read the ground truth file
  vtkSmartPointer<vtkXMLPolyDataReader> groundTruthReader =
    vtkSmartPointer<vtkXMLPolyDataReader>::New();
  groundTruthReader->SetFileName(groundTruthFilename.c_str());
  groundTruthReader->Update();

  vtkPolyData* groundTruthPolyData = groundTruthReader->GetOutput();

  // Check that the number of points match
  if(groundTruthPolyData->GetNumberOfPoints() != estimatedNormalsPolyData->GetNumberOfPoints())
    {
    cout << "Number of points does not match!" << endl;
    cout << "Estimated points: " << estimatedNormalsPolyData->GetNumberOfPoints() << endl;
    cout << "Ground truth points: " << groundTruthPolyData->GetNumberOfPoints() << endl;
    return EXIT_FAILURE;
    }

  // Get both sets of normals
  vtkDataArray* groundTruthNormals =
    groundTruthPolyData->GetPointData()->GetNormals();
  vtkDataArray* estimatedNormals =
    estimatedNormalsPolyData->GetPointData()->GetNormals();

  for(vtkIdType i = 0; i < groundTruthPolyData->GetNumberOfPoints(); i++)
    {
//     double gt[3];
//     double est[3];
//     estimatedNormals->GetTuple(i, est);
//     groundTruthNormals->GetTuple(i, gt);
    double* gt = groundTruthNormals->GetTuple(i);
    double* est = estimatedNormals->GetTuple(i);

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
