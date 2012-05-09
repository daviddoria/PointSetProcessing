cmake_minimum_required(VERSION 2.6)

PROJECT(PointSetProcessing)

SET(BuildPlugins OFF CACHE BOOL "Build plugins?")
if(BuildPlugins)
  FIND_PACKAGE(ParaView REQUIRED)
  INCLUDE(${PARAVIEW_USE_FILE})
  add_subdirectory(ParaViewPlugins)
else()
  FIND_PACKAGE(VTK REQUIRED)
  INCLUDE(${VTK_USE_FILE})
endif()

# Note: If you are using ParaView's VTK so you can build plugins and ParaView has not yet upgraded to VTK 6, I'm not sure what to do.
if("${VTK_VERSION}" VERSION_LESS 6.0)
  message("Your VTK version is too old! Must be greater than 6.0")
else()
  message("Valid VTK version (${VTK_VERSION}).")
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

if(NOT vtkInfovisBoost_LOADED)
  message(FATAL_ERROR "vtkInfovisBoost is required but not available.")
endif()

if(NOT vtkInfovisBoostGraphAlgorithms_LOADED)
  message(FATAL_ERROR "vtkInfovisBoostGraphAlgorithms is required but not available.")
endif()

SET(PointSetProcessingDir ${CMAKE_CURRENT_SOURCE_DIR})
SET(DataDir ./)

SET(BuildTesting OFF CACHE BOOL "Build testing?")
if(BuildTesting)
  message("Building tests...")
  add_subdirectory(Tests)
endif()


SET(BuildGUI OFF CACHE BOOL "Build GUI?")
if(BuildGUI)
  add_subdirectory(GUI)
endif()

SET(BuildExamples OFF CACHE BOOL "Build examples?")
if(BuildExamples)
  add_subdirectory(Examples)
endif()
