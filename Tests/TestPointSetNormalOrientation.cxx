#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include <limits>

#include "vtkPointSetNormalOrientation.h"

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
    std::cout << "Required arguments invalid!" << std::endl;
    std::cout << "Required arguments: InputFilename GroundTruthFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFileName = argv[1];
  std::string groundTruthFileName = argv[2];

  // Read the input file
  vtkSmartPointer<vtkXMLPolyDataReader> inputReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  inputReader->SetFileName(inputFileName.c_str());
  inputReader->Update();

  // Estimate normals
  vtkSmartPointer<vtkPointSetNormalOrientation> normalOrientation = vtkSmartPointer<vtkPointSetNormalOrientation>::New();
  normalOrientation->SetGraphFilterType(vtkPointSetNormalOrientation::RIEMANN_GRAPH);
  normalOrientation->SetInputConnection(inputReader->GetOutputPort());
  normalOrientation->Update();

  vtkPolyData* orientedNormalsPolyData = normalOrientation->GetOutput();

  // Read the ground truth file
  vtkSmartPointer<vtkXMLPolyDataReader> groundTruthReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  groundTruthReader->SetFileName(groundTruthFileName.c_str());
  groundTruthReader->Update();

  vtkPolyData* groundTruthPolyData = groundTruthReader->GetOutput();

  if(groundTruthPolyData->GetNumberOfPoints() != orientedNormalsPolyData->GetNumberOfPoints())
    {
    std::cout << "Number of points do not match!" << std::endl;
    std::cout << "Ground truth points: " << groundTruthPolyData->GetNumberOfPoints() << std::endl;
    std::cout << "Input points: " << orientedNormalsPolyData->GetNumberOfPoints() << std::endl;
    return EXIT_FAILURE;
    }

  // Get both sets of normals
  vtkDataArray* groundTruthNormals =
    groundTruthPolyData->GetPointData()->GetNormals();
  vtkDataArray* orientedNormals =
    orientedNormalsPolyData->GetPointData()->GetNormals();

  for(vtkIdType i = 0; i < groundTruthPolyData->GetNumberOfPoints(); i++)
    {
//     double gt[3];
//     double est[3];
//     orientedNormals->GetTuple(i, est);
//     groundTruthNormals->GetTuple(i, gt);

    double* gt = groundTruthNormals->GetTuple(i);
    double* est = orientedNormals->GetTuple(i);

    for(unsigned int p = 0; p < 3; p++)
      {
      if(!fuzzyCompare(gt[p], est[p]))
        {
        std::cout << "!fuzzyCompare(gt[p], est[p]))" << std::endl;
        std::cout << "gt: " << gt[p] << std::endl;
        std::cout << "est: " << est[p] << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  return EXIT_SUCCESS;
}
