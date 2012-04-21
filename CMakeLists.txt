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


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

if(NOT VTK_USE_BOOST)
  MESSAGE(SEND_ERROR "VTK must be built with VTK_USE_BOOST=ON.")
endif ()

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
