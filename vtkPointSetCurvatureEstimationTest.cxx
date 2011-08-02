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

#include "vtkPointSetCurvatureEstimation.h"

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
  
  if(!inputReader->GetOutput())
    {
    cout << "Error reading input file: " << inputFilename << endl;
    return EXIT_FAILURE;
    }
  
  //estimate normals
  vtkSmartPointer<vtkPointSetCurvatureEstimation> curvatureEstimation = 
      vtkSmartPointer<vtkPointSetCurvatureEstimation>::New();
  curvatureEstimation->SetInput(inputReader->GetOutput());
  curvatureEstimation->Update();
  
  vtkPolyData* curvatureEstimate = curvatureEstimation->GetOutput();

  //read the ground truth file
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

  vtkDoubleArray* groundTruthCurvature = 
      vtkDoubleArray::SafeDownCast ( groundTruth->GetPointData()->GetArray ( "Curvature" ) );
  vtkDoubleArray* estimatedCurvature = 
      vtkDoubleArray::SafeDownCast ( curvatureEstimate->GetPointData()->GetArray ( "Curvature" ) );
  
  for(unsigned int i = 0; i < groundTruth->GetNumberOfPoints(); i++)
    {
    double gc = groundTruthCurvature->GetValue ( i );
    double ec = estimatedCurvature->GetValue(i);
    
      
    if(!fuzzyCompare(gc,ec))
      {
      cout << "!fuzzyCompare(gc,ec)" << endl;
      cout << "gc: " << gc << endl;
      cout << "ec: " << ec << endl;
      return EXIT_FAILURE;
      }
    }
    
  return EXIT_SUCCESS;
}
