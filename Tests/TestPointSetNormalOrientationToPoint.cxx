#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include <limits>
#include <ctime>

#include "vtkPointSetNormalOrientationToPoint.h"

static void WritePolyData(vtkPolyData* polyData, const std::string& fileName);

template<class T>
bool fuzzyCompare(T a, T b)
{
  return fabs(a - b) < std::numeric_limits<T>::epsilon();
}

int main (int, char *[])
{
  // Without this line, the random numbers will be the same every iteration.
  vtkMath::RandomSeed(std::time(NULL));

  // Create a plane of points, each with a random normal.
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkFloatArray> normals = vtkSmartPointer<vtkFloatArray>::New();
  normals->SetNumberOfComponents(3);
  normals->SetName("Normals");
  for(unsigned int x = 0; x < 6; ++x)
    {
    for(unsigned int y = 0; y < 2; ++y)
      {
      points->InsertNextPoint(x,y,0);
      float randomValue = vtkMath::Random();
      if(randomValue < .5)
        {
        randomValue *= -1.0f;
        }
      float normal[3] = {0, 0, randomValue};
      normals->InsertNextTupleValue(normal);
      }
    }

  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
  polyData->SetPoints(points);
  polyData->GetPointData()->SetNormals(normals);

  WritePolyData(polyData, "input.vtp");

  // Orient the normals toward +z
  vtkSmartPointer<vtkPointSetNormalOrientationToPoint> orientationToPointFilter =
    vtkSmartPointer<vtkPointSetNormalOrientationToPoint>::New();
  orientationToPointFilter->SetInputData(polyData);
  double orientationPoint[3] = {0,0,10};
  orientationToPointFilter->SetOrientationPoint(orientationPoint);
  orientationToPointFilter->Update();

  WritePolyData(orientationToPointFilter->GetOutput(), "output.vtp");

  vtkFloatArray* newNormals = vtkFloatArray::SafeDownCast(orientationToPointFilter->GetOutput()->GetPointData()->GetNormals());

  // Check the angle between each output normal and the vector to the orientation point
  for(vtkIdType pointId = 0; pointId < polyData->GetNumberOfPoints(); ++pointId)
    {
    // Get the current point.
    double p[3];
    polyData->GetPoint(pointId, p);

    // Construct the vector between the point and the OrientationPoint
    double vectorToOrientationPoint[3] = {orientationPoint[0] - p[0], orientationPoint[1] - p[1], orientationPoint[2] - p[2]};

    double oldNormal[3];
    newNormals->GetTuple(pointId, oldNormal);

    // std::cout << "Old normal: " << OldNormal[0] << " " << OldNormal[1] << " " << OldNormal[2] << " ";
    if(vtkMath::Dot(oldNormal, vectorToOrientationPoint) < 0.0) // The normal is facing the "wrong" way.
      {
      cerr << "Error, normal " << pointId << " should have been flipped!" << endl;
      cerr << "Dot product is " << vtkMath::Dot(oldNormal, vectorToOrientationPoint) << endl;
      return EXIT_FAILURE;
      }
    }

  return EXIT_SUCCESS;
}

void WritePolyData(vtkPolyData* polyData, const std::string& fileName)
{
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetInputData(polyData);
  writer->SetFileName(fileName.c_str());
  writer->Write();
}
