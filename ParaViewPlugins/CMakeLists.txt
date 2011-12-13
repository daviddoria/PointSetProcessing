# FIND_PACKAGE(ParaView REQUIRED)
# INCLUDE(${PARAVIEW_USE_FILE})

ADD_PARAVIEW_PLUGIN(PointSetOutlierRemoval "1.0"
  SERVER_MANAGER_XML vtkPointSetOutlierRemoval.xml
  SERVER_MANAGER_SOURCES ${PointSetProcessingDir}/vtkPointSetOutlierRemoval.cxx
  )

ADD_PARAVIEW_PLUGIN(PointSetCurvatureEstimation "1.0"
  SERVER_MANAGER_XML vtkPointSetCurvatureEstimation.xml
  SERVER_MANAGER_SOURCES ${PointSetProcessingDir}/vtkPointSetCurvatureEstimation.cxx
  )

ADD_PARAVIEW_PLUGIN(PointSetNormalEstimation "1.0"
  SERVER_MANAGER_XML vtkPointSetNormalEstimation.xml
  SERVER_MANAGER_SOURCES ${PointSetProcessingDir}/vtkPointSetNormalEstimation.cxx
  )

ADD_PARAVIEW_PLUGIN(PointSetNormalOrientation "1.0"
  SERVER_MANAGER_XML vtkPointSetNormalOrientation.xml
  SERVER_MANAGER_SOURCES ${PointSetProcessingDir}/vtkPointSetNormalOrientation.cxx ${PointSetProcessingDir}/vtkRiemannianGraphFilter.cxx ${PointSetProcessingDir}/vtkEuclideanMinimumSpanningTree.cxx
  )
