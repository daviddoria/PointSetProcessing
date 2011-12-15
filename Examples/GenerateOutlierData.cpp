#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkMath.h"
#include "vtkCellArray.h"

#include "vtkPointSetOutlierRemoval.h"

int main (int argc, char *argv[])
{
  //create a bunch of clustered points
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  double spacing = .01;
  for(unsigned int i = 0; i < 10; i++)
    {
      for(unsigned int j = 0; j < 10; j++)
        {
          for(unsigned int k = 0; k < 10; k++)
            {
            points->InsertNextPoint(i*spacing, j*spacing, k*spacing);
            }
        }
    }
  
  //add some outlier points
  double val = .4;
  points->InsertNextPoint(val, val, val);
  points->InsertNextPoint(-val, val, val);
  points->InsertNextPoint(val, -val, val);
  
  //create cells
  vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
  for(unsigned int i = 0; i < points->GetNumberOfPoints(); i++)
  {
    vtkIdType id[1];
    id[0] = i;
    vertices->InsertNextCell ( 1, id );
  }
  
  vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);
  polydata->SetVerts(vertices);

  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName("Input.vtp");
  writer->SetInput(polydata);
  writer->Write();
  
  return 0;
}
