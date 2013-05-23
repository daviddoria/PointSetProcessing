PointSetProcessing
==================

A set of classes (vtkPointSetOutlierRemoval, vtkPointSetNormalEstimation, 
vtkPointSetNormalOrientation, vtkPointSetCurvatureEstimation, vtkEuclideanMinimumSpanningTree, 
and vtkRiemannianGraphFilter) to enable several basic operations on point sets. 
These classes are implemented as VTK filters.

Requirements
============
You must set Module_vtkInfovisBoost=ON and Module_vtkInfovisBoostGraphAlgorithms=ON
in VTK 6's CMake configuration before building VTK. (Of course, 
this required Boost to be available on your system).
