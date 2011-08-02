cmake_minimum_required(VERSION 2.6)
    if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)

PROJECT(vtkPointSetOutlierRemoval)
ENABLE_TESTING()

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ../)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

SET(BUILD_PARAVIEW_PLUGIN OFF CACHE BOOL "Build Paraview plugin?")

SET(PointSetProcessingDir ./)
SET(DataDir ./)

############ Paraview Plugins ###############
if(BUILD_PARAVIEW_PLUGIN)
  FIND_PACKAGE(ParaView REQUIRED)
  INCLUDE(${PARAVIEW_USE_FILE})

  ADD_PARAVIEW_PLUGIN(PointSetOutlierRemoval "1.0"
    SERVER_MANAGER_XML ${PointSetProcessingDir}/vtkPointSetOutlierRemoval.xml
    SERVER_MANAGER_SOURCES ${PointSetProcessingDir}/vtkPointSetOutlierRemoval.cxx
    )

  ADD_PARAVIEW_PLUGIN(PointSetCurvatureEstimation "1.0"
    SERVER_MANAGER_XML ${PointSetProcessingDir}/vtkPointSetCurvatureEstimation.xml
    SERVER_MANAGER_SOURCES ${PointSetProcessingDir}/vtkPointSetCurvatureEstimation.cxx
    )

  ADD_PARAVIEW_PLUGIN(PointSetNormalEstimation "1.0"
    SERVER_MANAGER_XML ${PointSetProcessingDir}/vtkPointSetNormalEstimation.xml
    SERVER_MANAGER_SOURCES ${PointSetProcessingDir}/vtkPointSetNormalEstimation.cxx
    )

  ADD_PARAVIEW_PLUGIN(PointSetNormalOrientation "1.0"
    SERVER_MANAGER_XML ${PointSetProcessingDir}/vtkPointSetNormalOrientation.xml
    SERVER_MANAGER_SOURCES ${PointSetProcessingDir}/vtkPointSetNormalOrientation.cxx ${PointSetProcessingDir}/vtkRiemannianGraphFilter.cxx ${PointSetProcessingDir}/vtkEuclideanMinimumSpanningTree.cxx
    )

endif(BUILD_PARAVIEW_PLUGIN)

############ VTK Filters ####################
ADD_EXECUTABLE(vtkPointSetOutlierRemovalExample ${PointSetProcessingDir}/vtkPointSetOutlierRemovalExample.cxx
${PointSetProcessingDir}/vtkPointSetOutlierRemoval.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetOutlierRemovalExample vtkHybrid )

ADD_EXECUTABLE(vtkPointSetCurvatureEstimationExample ${PointSetProcessingDir}/vtkPointSetCurvatureEstimationExample.cxx
${PointSetProcessingDir}/vtkPointSetCurvatureEstimation.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetCurvatureEstimationExample vtkHybrid )

ADD_EXECUTABLE(vtkPointSetNormalEstimationExample ${PointSetProcessingDir}/vtkPointSetNormalEstimationExample.cxx
${PointSetProcessingDir}/vtkPointSetNormalEstimation.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetNormalEstimationExample vtkHybrid )

ADD_EXECUTABLE(vtkPointSetNormalOrientationExample ${PointSetProcessingDir}/vtkPointSetNormalOrientationExample.cxx
${PointSetProcessingDir}/vtkPointSetNormalOrientation.cxx 
${PointSetProcessingDir}/vtkRiemannianGraphFilter.cxx 
${PointSetProcessingDir}/vtkEuclideanMinimumSpanningTree.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetNormalOrientationExample vtkHybrid vtkInfovis)



############### Tests ########################
# Outlier Removal
ADD_EXECUTABLE(vtkPointSetOutlierRemovalTest ${PointSetProcessingDir}/vtkPointSetOutlierRemovalTest.cxx ${PointSetProcessingDir}/vtkPointSetOutlierRemoval.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetOutlierRemovalTest vtkHybrid )
ADD_TEST(vtkPointSetOutlierRemovalTest vtkPointSetOutlierRemovalTest ${DataDir}/vtkPointSetOutlierRemoval_Input.vtp ${DataDir}/vtkPointSetOutlierRemoval_GroundTruth.vtp)

# Curvature Estimation
ADD_EXECUTABLE(vtkPointSetCurvatureEstimationTest ${PointSetProcessingDir}/vtkPointSetCurvatureEstimationTest.cxx 
${PointSetProcessingDir}/vtkPointSetNormalEstimation.cxx
${PointSetProcessingDir}/vtkPointSetCurvatureEstimation.cxx
)
TARGET_LINK_LIBRARIES(vtkPointSetCurvatureEstimationTest vtkHybrid )
ADD_TEST(vtkPointSetCurvatureEstimationTest vtkPointSetCurvatureEstimationTest ${DataDir}/vtkPointSetCurvatureEstimation_Input.vtp ${DataDir}/vtkPointSetCurvatureEstimation_GroundTruth.vtp)

# Normal Estimation
ADD_EXECUTABLE(vtkPointSetNormalEstimationTest ${PointSetProcessingDir}/vtkPointSetNormalEstimationTest.cxx 
${PointSetProcessingDir}/vtkPointSetNormalEstimation.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetNormalEstimationTest vtkHybrid )
ADD_TEST(vtkPointSetNormalEstimationTest vtkPointSetNormalEstimationTest ${DataDir}/vtkPointSetNormalEstimation_Input.vtp ${DataDir}/vtkPointSetNormalEstimation_GroundTruth.vtp)

# Normal Orientation
ADD_EXECUTABLE(vtkPointSetNormalOrientationTest ${PointSetProcessingDir}/vtkPointSetNormalOrientationTest.cxx 
${PointSetProcessingDir}/vtkPointSetNormalOrientation.cxx
${PointSetProcessingDir}/vtkEuclideanMinimumSpanningTree.cxx  
${PointSetProcessingDir}/vtkRiemannianGraphFilter.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetNormalOrientationTest vtkHybrid vtkInfovis)
ADD_TEST(vtkPointSetNormalOrientationTest vtkPointSetNormalOrientationTest ${DataDir}/vtkPointSetNormalOrientation_Input.vtp ${DataDir}/vtkPointSetNormalOrientation_GroundTruth.vtp)
