#include <vtkBoundingBox.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkIdList.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkKdTreePointLocator.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkPlane.h>
#include <vtkPolyData.h>

#include "vtkPointSetNormalEstimation.h"
#include "vtkPointSetCurvatureEstimation.h"

vtkStandardNewMacro(vtkPointSetCurvatureEstimation);

vtkPointSetCurvatureEstimation::vtkPointSetCurvatureEstimation()
{
  //Set defaults.
  this->UseAutoRadius = true;
  this->Radius = 1.0;
}


/*
void vtkPointSetCurvatureEstimation::SetRadius(const double r)
{
  //Set the radius of the nearest neighbor sphere.
  this->UseAutoRadius = false;
  this->Radius = r;
  
  this->Modified();
}
*/

void vtkPointSetCurvatureEstimation::SetAutoRadius(vtkPolyData* input)
{
  //Compute and set the radius of the nearest neighbor sphere.
  
  vtkBoundingBox box;
  //Construct the point set bounding box.
  for(vtkIdType i = 0; i < input->GetNumberOfPoints(); i++)
    {
    double p[3];
    input->GetPoint(i,p);
    box.AddPoint(p);
    }
  
  //Set the radius to a heuristic on the bounding box diagonal length.
  this->Radius = .2 * box.GetDiagonalLength();
  
  this->Modified();
}

int vtkPointSetCurvatureEstimation::RequestData(vtkInformation *vtkNotUsed(request),
                                 vtkInformationVector **inputVector,
                                 vtkInformationVector *outputVector)
{
  // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Compute a reasonable radius automatically if requested
  if(UseAutoRadius)
    {
    SetAutoRadius(input);
    }
    
  // Create curvature array
  vtkSmartPointer<vtkDoubleArray> Curvature = vtkSmartPointer<vtkDoubleArray>::New();
  Curvature->SetNumberOfComponents( 1 );
  Curvature->SetNumberOfTuples( input->GetNumberOfPoints() );
  Curvature->SetName( "Curvature" );
  
  // Get the normals
  //vtkDoubleArray* Normals = vtkDoubleArray::SafeDownCast(input->GetPointData()->GetNormals());
  vtkFloatArray* normals = vtkFloatArray::SafeDownCast(input->GetPointData()->GetNormals());

  // If normals are not present or the number of normals does not match the number of points, fail
  if(!(normals && (normals->GetNumberOfTuples() == input->GetNumberOfPoints())))
    {
    vtkErrorMacro("The input points must have normals!");
    return 0;
    }
    
  // Build a KDTree
  vtkSmartPointer<vtkKdTreePointLocator> KDTree = vtkSmartPointer<vtkKdTreePointLocator>::New();
  KDTree->SetDataSet(input);
  KDTree->BuildLocator();
  
  // For each point in the data, get the points
  for(vtkIdType i = 0; i < input->GetNumberOfPoints(); i++)
    {
    // This iteration through the loop computes the ith points' curvature
    double point[3];
    input->GetPoint(i, point);

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    double normal[3];
    normals->GetTuple(i,normal);
    plane->SetNormal(normal);
    plane->SetOrigin(point);
      
    vtkSmartPointer<vtkIdList> neighborIds = vtkSmartPointer<vtkIdList>::New();
    KDTree->FindPointsWithinRadius(this->Radius, point, neighborIds);
    
    // Put all the points that are within the specified radius into a vtkPoints object
    vtkSmartPointer<vtkPoints> neighbors = vtkSmartPointer<vtkPoints>::New();
    for(vtkIdType p = 0; p < neighborIds->GetNumberOfIds(); p++)
      {
      double neighbor[3];
      input->GetPoint(neighborIds->GetId(p), neighbor);
      neighbors->InsertNextPoint(neighbor);
      }
    
    //compute the average distance from all of the points inside the radius to the plane
    double TotalDistance = 0.0;
    for(vtkIdType p = 0; p < neighborIds->GetNumberOfIds(); p++)
      {
      double neighbor[3];
      input->GetPoint(neighborIds->GetId(p), neighbor);
      double dist = vtkPlane::DistanceToPlane(neighbor, plane->GetNormal(), plane->GetOrigin());
      TotalDistance += dist;
      }
      
    double AverageDistance = TotalDistance/static_cast<double>(neighborIds->GetNumberOfIds());
    Curvature->SetValue(i, AverageDistance);
    }
  
  output->ShallowCopy(input);
  
  // Normalize curvature (0,1)
  // Find max value
  double maxValue = 0.0;
  for(vtkIdType i = 0; i < Curvature->GetNumberOfTuples(); i++)
    {
    double val = Curvature->GetValue(i);
    if(val > maxValue)
      {
      maxValue = val;
      }
    }
    
  // Divide each value by the max value
  for(vtkIdType i = 0; i < Curvature->GetNumberOfTuples(); i++)
    {
    double val = Curvature->GetValue(i);
    Curvature->SetValue(i, val/maxValue);
    }
    
  output->GetPointData()->AddArray(Curvature);
  
  return 1;
}

////////// External Operators /////////////

void vtkPointSetCurvatureEstimation::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "Neighbor sphere radius: " << this->Radius << vtkstd::endl;
  os << indent << "Use auto radius: " << this->UseAutoRadius << vtkstd::endl;
}

void ComputeCenterOfMass(vtkPoints* points, double* Center)
{
  Center[0] = 0.0;
  Center[1] = 0.0;
  Center[2] = 0.0;
    
  for(vtkIdType i = 0; i < points->GetNumberOfPoints(); i++)
  {
    double point[3];
    points->GetPoint(i, point);
    
    Center[0] += point[0];
    Center[1] += point[1];
    Center[2] += point[2];
  }
  
  double NumberOfPoints = static_cast<double>(points->GetNumberOfPoints());
  Center[0] = Center[0]/NumberOfPoints;
  Center[1] = Center[1]/NumberOfPoints;
  Center[2] = Center[2]/NumberOfPoints;
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

#if 0
void BestFitPlane(vtkPoints *points, vtkPlane *BestPlane)
{
  vtkIdType NumPoints = points->GetNumberOfPoints();
  double dNumPoints = static_cast<double>(NumPoints);
  
  //find the center of mass of the points
  double Center[3];
  ComputeCenterOfMass(points, Center);
  //vtkstd::cout << "Center of mass: " << Center[0] << " " << Center[1] << " " << Center[2] << vtkstd::endl;
  
  //Compute sample covariance matrix
  double **a = create_matrix<double> ( 3,3 );
  a[0][0] = 0; a[0][1] = 0;  a[0][2] = 0;
  a[1][0] = 0; a[1][1] = 0;  a[1][2] = 0;
  a[2][0] = 0; a[2][1] = 0;  a[2][2] = 0;
  
  for(unsigned int pointId = 0; pointId < NumPoints; pointId++ )
  {
    double x[3];
    double xp[3];
    points->GetPoint(pointId, x);
    xp[0] = x[0] - Center[0]; 
    xp[1] = x[1] - Center[1]; 
    xp[2] = x[2] - Center[2];
    for (unsigned int i = 0; i < 3; i++)
    {
      a[0][i] += xp[0] * xp[i];
      a[1][i] += xp[1] * xp[i];
      a[2][i] += xp[2] * xp[i];
    }
  }
  
    //divide by N-1
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
  
  //Set the plane normal to the smallest eigen vector
  BestPlane->SetNormal(eigvec[0][2], eigvec[1][2], eigvec[2][2]);
  
  //cleanup
  free_matrix(eigvec);
  free_matrix(a);
  
  //Set the plane origin to the center of mass
  BestPlane->SetOrigin(Center[0], Center[1], Center[2]);

}
#endif
