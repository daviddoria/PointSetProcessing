#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include <limits>

#include "vtkPointSetCurvatureEstimation.h"

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

  if(!inputReader->GetOutput())
    {
    cout << "Error reading input file: " << inputFilename << endl;
    return EXIT_FAILURE;
    }

  // Estimate normals
  vtkSmartPointer<vtkPointSetCurvatureEstimation> curvatureEstimation =
    vtkSmartPointer<vtkPointSetCurvatureEstimation>::New();
  curvatureEstimation->SetInputConnection(inputReader->GetOutputPort());
  curvatureEstimation->Update();

  vtkPolyData* curvatureEstimate = curvatureEstimation->GetOutput();

  // Read the ground truth file
  vtkSmartPointer<vtkXMLPolyDataReader> groundTruthReader =
    vtkSmartPointer<vtkXMLPolyDataReader>::New();
  groundTruthReader->SetFileName(groundTruthFilename.c_str());
  groundTruthReader->Update();

  vtkPolyData* groundTruth = groundTruthReader->GetOutput();

  if(!groundTruth)
    {
    cout << "Error reading ground truth file: " << groundTruthFilename << endl;
    return EXIT_FAILURE;
    }

  if(groundTruth->GetNumberOfPoints() != curvatureEstimate->GetNumberOfPoints())
    {
    cout << "Number of points does not match!" << endl;
    cout << "GroundTruth: " << groundTruth->GetNumberOfPoints() << endl;
    cout << "Estimate: " << curvatureEstimate->GetNumberOfPoints() << endl;
    return EXIT_FAILURE;
    }

  vtkDataArray* groundTruthCurvature =
    groundTruth->GetPointData()->GetArray("Curvature");
  vtkDataArray* estimatedCurvature =
    curvatureEstimate->GetPointData()->GetArray("Curvature");

  for(vtkIdType i = 0; i < groundTruth->GetNumberOfPoints(); i++)
    {
//     double gc = groundTruthCurvature->GetValue ( i );
//     double ec = estimatedCurvature->GetValue(i);
    double* gc = groundTruthCurvature->GetTuple(i);
    double* ec = estimatedCurvature->GetTuple(i);

    if(!fuzzyCompare(*gc,*ec))
      {
      cout << "!fuzzyCompare(gc,ec)" << endl;
      cout << "gc: " << *gc << endl;
      cout << "ec: " << *ec << endl;
      return EXIT_FAILURE;
      }
    }

  return EXIT_SUCCESS;
}
