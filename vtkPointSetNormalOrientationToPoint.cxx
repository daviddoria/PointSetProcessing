#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkIdList.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkIntArray.h>
#include <vtkPoints.h>
#include <vtkObjectFactory.h> //for new() macro
#include <vtkPointData.h>
#include <vtkKdTreePointLocator.h>
#include <vtkMath.h>
#include <vtkPolyData.h>

#include "vtkPointSetNormalOrientationToPoint.h"

vtkStandardNewMacro(vtkPointSetNormalOrientationToPoint);

vtkPointSetNormalOrientationToPoint::vtkPointSetNormalOrientationToPoint()
{
  this->OrientationPoint[0] = 0.0d;
  this->OrientationPoint[1] = 0.0d;
  this->OrientationPoint[2] = 0.0d;
}


int vtkPointSetNormalOrientationToPoint::RequestData(vtkInformation *vtkNotUsed(request),
                                             vtkInformationVector **inputVector,
                                             vtkInformationVector *outputVector)
{
  // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkFloatArray* oldNormals = vtkFloatArray::SafeDownCast(input->GetPointData()->GetNormals());

  vtkSmartPointer<vtkFloatArray> orientedNormals = vtkSmartPointer<vtkFloatArray>::New();
  orientedNormals->SetName("Normals");
  orientedNormals->SetNumberOfComponents(3); //3d normals (ie x,y,z)
  orientedNormals->SetNumberOfTuples(input->GetNumberOfPoints());

  // Traverse the points and flip normals if necessary
  for(vtkIdType pointId = 0; pointId < input->GetNumberOfPoints(); ++pointId)
  {
    // Get the current point.
    double p[3];
    input->GetPoint(pointId, p);
    // Construct the vector between the point and the OrientationPoint
    double vectorToOrientationPoint[3] = {this->OrientationPoint[0] - p[0], this->OrientationPoint[1] - p[1], this->OrientationPoint[2] - p[2]};

    double oldNormal[3];
    oldNormals->GetTuple(pointId, oldNormal);

    // std::cout << "Old normal: " << OldNormal[0] << " " << OldNormal[1] << " " << OldNormal[2] << " ";
    if(vtkMath::Dot(oldNormal, vectorToOrientationPoint) < 0.0) // The normal is facing the "wrong" way.
      {
      // Flip the normal
      vtkMath::MultiplyScalar(oldNormal, -1.0f);
      }

    orientedNormals->SetTuple(pointId, oldNormal);
  }

  output->ShallowCopy(input);
  output->GetPointData()->SetNormals(orientedNormals);
  return 1;
}

////////// External Operators /////////////

void vtkPointSetNormalOrientationToPoint::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "Orientation point: (" << this->OrientationPoint[0] << ", "
               << this->OrientationPoint[1] << ", " << this->OrientationPoint[2] << ")" << std::endl;
  
}
