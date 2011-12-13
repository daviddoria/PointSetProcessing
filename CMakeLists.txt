cmake_minimum_required(VERSION 2.6)

PROJECT(PointSetProcessing)
ENABLE_TESTING()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

if(NOT VTK_USE_BOOST)
  MESSAGE(SEND_ERROR "VTK must be buile with VTK_USE_BOOST=ON.")
endif ()

SET(PointSetProcessingDir ${CMAKE_CURRENT_SOURCE_DIR})
SET(DataDir ./)

############ VTK Filters ####################
ADD_EXECUTABLE(vtkPointSetOutlierRemovalExample ${PointSetProcessingDir}/vtkPointSetOutlierRemovalExample.cxx
${PointSetProcessingDir}/vtkPointSetOutlierRemoval.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetOutlierRemovalExample ${VTK_LIBRARIES} )

ADD_EXECUTABLE(vtkPointSetCurvatureEstimationExample ${PointSetProcessingDir}/vtkPointSetCurvatureEstimationExample.cxx
${PointSetProcessingDir}/vtkPointSetCurvatureEstimation.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetCurvatureEstimationExample ${VTK_LIBRARIES} )

ADD_EXECUTABLE(vtkPointSetNormalEstimationExample ${PointSetProcessingDir}/vtkPointSetNormalEstimationExample.cxx
${PointSetProcessingDir}/vtkPointSetNormalEstimation.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetNormalEstimationExample ${VTK_LIBRARIES} )

ADD_EXECUTABLE(vtkPointSetNormalOrientationExample ${PointSetProcessingDir}/vtkPointSetNormalOrientationExample.cxx
${PointSetProcessingDir}/vtkPointSetNormalOrientation.cxx 
${PointSetProcessingDir}/vtkRiemannianGraphFilter.cxx 
${PointSetProcessingDir}/vtkEuclideanMinimumSpanningTree.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetNormalOrientationExample ${VTK_LIBRARIES})

SET(BuildTesting OFF CACHE BOOL "Build testing?")
if(BuildTesting)
  add_subdirectory(Tests)
endif()

SET(BuildPlugins OFF CACHE BOOL "Build plugins?")
if(BuildPlugins)
  add_subdirectory(ParaViewPlugins)
endif()
