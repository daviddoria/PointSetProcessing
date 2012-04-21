#include <vtkCellArray.h>
#include <vtkCommand.h>
#include <vtkFloatArray.h>
#include <vtkIdList.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkKdTree.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkPlane.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include "vtkPointSetNormalEstimation.h"

vtkStandardNewMacro(vtkPointSetNormalEstimation);

vtkPointSetNormalEstimation::vtkPointSetNormalEstimation()
{
  this->NumberOfNeighbors = 4;
  this->Radius = 1.0;
  this->Mode = RADIUS;
  this->IterateEvent = vtkCommand::UserEvent + 1;
}

void vtkPointSetNormalEstimation::SetModeToFixedNumber()
{
  this->Mode = FIXED_NUMBER;
}

void vtkPointSetNormalEstimation::SetModeToRadius()
{
  this->Mode = RADIUS;
}

int vtkPointSetNormalEstimation::RequestData(vtkInformation *vtkNotUsed(request),
                                 vtkInformationVector **inputVector,
                                 vtkInformationVector *outputVector)
{
  // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Create normal array
  vtkSmartPointer<vtkFloatArray> normalArray = vtkSmartPointer<vtkFloatArray>::New();
  normalArray->SetNumberOfComponents( 3 );
  normalArray->SetNumberOfTuples( input->GetNumberOfPoints() );
  normalArray->SetName( "Normals" );

  vtkSmartPointer<vtkKdTree> kDTree = vtkSmartPointer<vtkKdTree>::New();
  kDTree->BuildLocatorFromPoints(input->GetPoints());

  // std::cout << "this->Radius: " << this->Radius << std::endl;

  // Estimate the normal at each point.
  for(vtkIdType pointId = 0; pointId < input->GetNumberOfPoints(); ++pointId)
    {
    this->InvokeEvent(this->IterateEvent, &pointId);
    double point[3];
    input->GetPoint(pointId, point);

    vtkSmartPointer<vtkIdList> neighborIds = vtkSmartPointer<vtkIdList>::New();
    if(this->Mode == FIXED_NUMBER)
      {
      kDTree->FindClosestNPoints(this->NumberOfNeighbors, point, neighborIds);
      }
    else if(this->Mode == RADIUS)
      {
      kDTree->FindPointsWithinRadius(this->Radius, point, neighborIds);
      // If there are not at least 3 points within the specified radius (the current
      // point gets included in the neighbors set), a plane is not defined. Instead,
      // force it to use 3 points.
      if(neighborIds->GetNumberOfIds() < 3)
        {
        kDTree->FindClosestNPoints(3, point, neighborIds);
        }
      }

    vtkSmartPointer<vtkPlane> bestPlane = vtkSmartPointer<vtkPlane>::New();
    BestFitPlane(input->GetPoints(), bestPlane, neighborIds);
    double normal[3];
    bestPlane->GetNormal(normal);
    normalArray->SetTuple( pointId, normal ) ;
    }

  input->GetPointData()->SetNormals(normalArray);

  output->ShallowCopy(input);

  return 1;
}

////////// External Operators /////////////

void vtkPointSetNormalEstimation::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "NumberOfNeighbors: " << this->NumberOfNeighbors << endl;
  os << indent << "Radius: " << this->Radius << endl;
  os << indent << "Mode: " << this->Mode << endl;
}

void CenterOfMass(vtkPoints* points, double* center, vtkIdList* idsToUse)
{
  // Compute the center of mass of a set of points.
  center[0] = 0.0;
  center[1] = 0.0;
  center[2] = 0.0;

  for(vtkIdType i = 0; i < idsToUse->GetNumberOfIds(); i++)
    {
    double point[3];
    points->GetPoint(idsToUse->GetId(i), point);

    center[0] += point[0];
    center[1] += point[1];
    center[2] += point[2];
    }

  double numberOfPoints = static_cast<double>(idsToUse->GetNumberOfIds());
  center[0] = center[0]/numberOfPoints;
  center[1] = center[1]/numberOfPoints;
  center[2] = center[2]/numberOfPoints;
}

void CenterOfMass(vtkPoints* points, double* center)
{
  // Compute the center of mass of a set of points.
  center[0] = 0.0;
  center[1] = 0.0;
  center[2] = 0.0;

  for(vtkIdType i = 0; i < points->GetNumberOfPoints(); i++)
    {
    double point[3];
    points->GetPoint(i, point);

    center[0] += point[0];
    center[1] += point[1];
    center[2] += point[2];
    }

  double numberOfPoints = static_cast<double>(points->GetNumberOfPoints());
  center[0] = center[0]/numberOfPoints;
  center[1] = center[1]/numberOfPoints;
  center[2] = center[2]/numberOfPoints;
}

/* allocate memory for an nrow x ncol matrix */
template<class TReal>
    TReal **create_matrix ( long nrow, long ncol )
{
  typedef TReal* TRealPointer;
  TReal **m = new TRealPointer[nrow];

  TReal* block = ( TReal* ) calloc ( nrow*ncol, sizeof ( TReal ) );
  m[0] = block;
  for ( int row = 1; row < nrow; ++row )
    {
    m[ row ] = &block[ row * ncol ];
    }
  return m;
}

/* free a TReal matrix allocated with matrix() */
template<class TReal>
    void free_matrix ( TReal **m )
{
  free ( m[0] );
  delete[] m;
}

void BestFitPlane(vtkPoints* points, vtkPlane* bestPlane)
{
  // Compute the best fit (least squares) plane through a set of points.
  vtkIdType numPoints = points->GetNumberOfPoints();
  double dNumPoints = static_cast<double>(numPoints);

  // Find the center of mass of the points
  double center[3];
  CenterOfMass(points, center);
  // std::cout << "Center of mass: " << Center[0] << " " << Center[1] << " " << Center[2] << std::endl;

  //Compute sample covariance matrix
  double **a = create_matrix<double> ( 3,3 );
  a[0][0] = 0; a[0][1] = 0;  a[0][2] = 0;
  a[1][0] = 0; a[1][1] = 0;  a[1][2] = 0;
  a[2][0] = 0; a[2][1] = 0;  a[2][2] = 0;

  for(vtkIdType pointId = 0; pointId < numPoints; pointId++ )
    {
    double x[3];
    double xp[3];
    points->GetPoint(pointId, x);
    xp[0] = x[0] - center[0];
    xp[1] = x[1] - center[1];
    xp[2] = x[2] - center[2];
    for (unsigned int i = 0; i < 3; i++)
      {
      a[0][i] += xp[0] * xp[i];
      a[1][i] += xp[1] * xp[i];
      a[2][i] += xp[2] * xp[i];
      }
    }

  // Divide by N-1
  for(unsigned int i = 0; i < 3; i++)
    {
    a[0][i] /= dNumPoints-1;
    a[1][i] /= dNumPoints-1;
    a[2][i] /= dNumPoints-1;
    }

  // Extract eigenvectors from covariance matrix
  double **eigvec = create_matrix<double> ( 3,3 );

  double eigval[3];
  vtkMath::Jacobi(a,eigval,eigvec);

  //Jacobi iteration for the solution of eigenvectors/eigenvalues of a 3x3 real symmetric matrix. Square 3x3 matrix a; output eigenvalues in w; and output eigenvectors in v. Resulting eigenvalues/vectors are sorted in decreasing order; eigenvectors are normalized.

  // The normal is the direction of the smallest eigen vector.
  double normal[3] = {eigvec[0][2], eigvec[1][2], eigvec[2][2]};

  // Make sure the normal is normalized.
  vtkMath::Normalize(normal);

  // Set the plane normal 
  bestPlane->SetNormal(normal);

  // Cleanup
  free_matrix(eigvec);
  free_matrix(a);

  // Set the plane origin to the center of mass
  bestPlane->SetOrigin(center[0], center[1], center[2]);

}

void BestFitPlane(vtkPoints* points, vtkPlane* bestPlane, vtkIdList* idsToUse)
{
  // Compute the best fit (least squares) plane through a set of points.
  vtkIdType numPoints = idsToUse->GetNumberOfIds();
  double dNumPoints = static_cast<double>(numPoints);

  // Find the center of mass of the points
  double center[3];
  CenterOfMass(points, center, idsToUse);
  // std::cout << "Center of mass: " << Center[0] << " " << Center[1] << " " << Center[2] << std::endl;

  //Compute sample covariance matrix
  double **a = create_matrix<double> ( 3,3 );
  a[0][0] = 0; a[0][1] = 0;  a[0][2] = 0;
  a[1][0] = 0; a[1][1] = 0;  a[1][2] = 0;
  a[2][0] = 0; a[2][1] = 0;  a[2][2] = 0;

  for(vtkIdType pointId = 0; pointId < numPoints; pointId++ )
    {
    double x[3];
    double xp[3];
    points->GetPoint(idsToUse->GetId(pointId), x);
    xp[0] = x[0] - center[0];
    xp[1] = x[1] - center[1];
    xp[2] = x[2] - center[2];
    for (unsigned int i = 0; i < 3; i++)
      {
      a[0][i] += xp[0] * xp[i];
      a[1][i] += xp[1] * xp[i];
      a[2][i] += xp[2] * xp[i];
      }
    }

  // Divide by N-1
  for(unsigned int i = 0; i < 3; i++)
    {
    a[0][i] /= dNumPoints-1;
    a[1][i] /= dNumPoints-1;
    a[2][i] /= dNumPoints-1;
    }

  // Extract eigenvectors from covariance matrix
  double **eigvec = create_matrix<double> ( 3,3 );

  double eigval[3];
  vtkMath::Jacobi(a,eigval,eigvec);

  //Jacobi iteration for the solution of eigenvectors/eigenvalues of a 3x3 real symmetric matrix. Square 3x3 matrix a; output eigenvalues in w; and output eigenvectors in v. Resulting eigenvalues/vectors are sorted in decreasing order; eigenvectors are normalized.

  // Set the plane normal to the smallest eigen vector
  bestPlane->SetNormal(eigvec[0][2], eigvec[1][2], eigvec[2][2]);

  // Cleanup
  free_matrix(eigvec);
  free_matrix(a);

  // Set the plane origin to the center of mass
  bestPlane->SetOrigin(center[0], center[1], center[2]);

}
