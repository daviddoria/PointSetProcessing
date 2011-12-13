FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT4_WRAP_UI(PointSetProcessingUI PointSetProcessingWidget.ui)
QT4_WRAP_CPP(PointSetProcessingMOC PointSetProcessingWidget.h VTKComputationThread.h)

add_library(libPointSetProcessing ../vtkPointSetNormalEstimation.cxx)

ADD_EXECUTABLE(PointSetProcessing PointSetProcessing.cpp PointSetProcessingWidget.cpp ${PointSetProcessingUI} ${PointSetProcessingMOC})
TARGET_LINK_LIBRARIES(PointSetProcessing ${QT_LIBRARIES} ${VTK_LIBRARIES} QVTK libPointSetProcessing)
