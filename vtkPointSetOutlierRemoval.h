/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkOutlierRemoval.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPointSetOutlierRemoval - Remove outliers from a pointset
// .SECTION Description
// vtkPointSetOutlierRemoval - Remove outliers from a pointset

#ifndef __vtkPointSetOutlierRemoval_h
#define __vtkPointSetOutlierRemoval_h

#include "vtkPolyDataAlgorithm.h"

class vtkTransform;
class vtkImageData;

#include "vtkSmartPointer.h"

#include <algorithm>

class vtkPointSetOutlierRemoval : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPointSetOutlierRemoval, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPointSetOutlierRemoval *New();

  vtkSetClampMacro(PercentToRemove,double,0.0,1.0);

protected:
  vtkPointSetOutlierRemoval(){}
  ~vtkPointSetOutlierRemoval(){}

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkPointSetOutlierRemoval(const vtkPointSetOutlierRemoval&);  // Not implemented.
  void operator=(const vtkPointSetOutlierRemoval&);  // Not implemented.

  double PercentToRemove; //stored as a value 0 to 1. I.e. PercentToRemove=.5 will remove 50% of the points

};

  template <typename T>
      struct NumberedItem
{
  unsigned int index;
  T Item;
};

  template <typename T>
      bool operator<(NumberedItem<T> NI1, NumberedItem<T> NI2)
{
    //return NI1.index < NI2.index;
  return NI1.Item < NI2.Item;
}

  template <typename T>
      std::vector<unsigned int> ParallelSortIndices(const std::vector<T> &Things, bool descending = false)
{
    //this function returns the new order of the indices after the sort
  std::vector<NumberedItem<T> > Pairs(Things.size());
    //vector<unsigned int> Indices(Things.size());
  for(unsigned int i = 0; i < Things.size(); i++)
  {
    Pairs[i].index = i;
    Pairs[i].Item = Things[i];
  }

  std::sort(Pairs.begin(), Pairs.end());

  std::vector<unsigned int> SortedIndices(Things.size());

    for(unsigned int i = 0; i < Things.size(); i++)
      SortedIndices[i] = Pairs[i].index;

    /*unsigned int counter = 0;
    for(unsigned int i = Things.size(); i >= 0; i--)
      SortedIndices[counter] = Pairs[i].index;
  */

  return SortedIndices;

}
#endif
