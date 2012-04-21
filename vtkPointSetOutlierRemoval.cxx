/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkPointSetOutlierRemoval.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPointSetOutlierRemoval.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "vtkImageData.h"
#include "vtkDelaunay2D.h"
#include "vtkSmartPointer.h"
#include "vtkTransform.h"
#include "vtkKdTreePointLocator.h"
#include "vtkMath.h"
#include "vtkIdList.h"

#include <sstream>

vtkStandardNewMacro(vtkPointSetOutlierRemoval);

int vtkPointSetOutlierRemoval::RequestData(vtkInformation *vtkNotUsed(request),
                                 vtkInformationVector **inputVector,
                                 vtkInformationVector *outputVector)
{

  // Get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the ouptut
  vtkPolyData *output = vtkPolyData::SafeDownCast(
		  outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  // Get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Create the tree
  vtkSmartPointer<vtkKdTreePointLocator> kdTree = vtkSmartPointer<vtkKdTreePointLocator>::New();
  kdTree->SetDataSet(input);
  kdTree->BuildLocator();

  std::vector<double> distances(input->GetNumberOfPoints());

  for(vtkIdType i = 0; i < input->GetNumberOfPoints(); i++)
    {
    double queryPoint[3];
    input->GetPoint(i, queryPoint);

    /* can't do this - it will find exactly the same point
    vtkIdType ID = KDTree->FindClosestPoint(QueryPoint);
    double ClosestPoint[3];
    input->GetPoint(i, ClosestPoint);
    */

    // Find the 2 closest points to (0,0,0)
    vtkSmartPointer<vtkIdList> Result = vtkSmartPointer<vtkIdList>::New();
    kdTree->FindClosestNPoints(2, queryPoint, Result);

    int point_ind = static_cast<int>(Result->GetId(1));
    double ClosestPoint[3];
    input->GetPoint(point_ind, ClosestPoint);

    double ClosestPointDistance = vtkMath::Distance2BetweenPoints(queryPoint, ClosestPoint);
    distances[i] = ClosestPointDistance;
    }

  // Compute points to be added
  std::vector<unsigned int> Indices = ParallelSortIndices(distances);

  unsigned int numberToKeep = static_cast<unsigned int>((1-this->PercentToRemove) * input->GetNumberOfPoints());
  //std::cout << "Number of input points: " << input->GetNumberOfPoints() << std::endl;
  //std::cout << "Percent to remove: " << this->PercentToRemove << std::endl;
  //std::cout << "Number of points to keep: " << NumberToKeep << std::endl;

  //add the points that should be kept to a new polydata
  vtkSmartPointer<vtkPoints> outputPoints = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> outputVertices = vtkSmartPointer<vtkCellArray>::New();
  for(unsigned int i = 0; i < numberToKeep; i++)
  {
    //std::cout << "Dist: " << Distances[Indices[i]] << std::endl;
    double p[3];
    input->GetPoint(Indices[i], p);
    vtkIdType pid[1];
    pid[0] = outputPoints->InsertNextPoint(p);
    outputVertices->InsertNextCell ( 1, pid );
  }

  //std::cout << "Number of points kept: " << OutputPoints->GetNumberOfPoints() << std::endl;

  vtkSmartPointer<vtkPolyData> OutputPolydata = vtkSmartPointer<vtkPolyData>::New();
  OutputPolydata->SetPoints(outputPoints);
  OutputPolydata->SetVerts(outputVertices);
  //std::cout << "Number of points kept: " << OutputPolydata->GetNumberOfPoints() << std::endl;

  output->ShallowCopy(OutputPolydata);

  return 1;
}


//----------------------------------------------------------------------------
void vtkPointSetOutlierRemoval::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

