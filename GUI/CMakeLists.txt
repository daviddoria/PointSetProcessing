FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

add_subdirectory(Widgets)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(./Widgets)

QT4_WRAP_UI(PointSetProcessingUI PointSetProcessingWidget.ui
Widgets/LabeledSlider.ui Widgets/CoordinateEditWidget.ui)
QT4_WRAP_CPP(PointSetProcessingMOC PointSetProcessingWidget.h VTKComputationThread.h
Widgets/LabeledSlider.h Widgets/FloatSlider.h Widgets/CoordinateEditWidget.h)

add_library(libPointSetProcessing ../vtkPointSetNormalEstimation.cxx ../vtkPointSetNormalOrientation.cxx ../vtkRiemannianGraphFilter.cxx ../vtkEuclideanMinimumSpanningTree.cxx
../vtkPointSetNormalOrientationToPoint.cxx)

add_executable(PointSetProcessing PointSetProcessing.cpp PointSetProcessingWidget.cpp
Widgets/LabeledSlider.cpp Widgets/FloatSlider.cpp Widgets/CoordinateEditWidget.cpp
${PointSetProcessingUI} ${PointSetProcessingMOC})
target_link_libraries(PointSetProcessing ${QT_LIBRARIES} ${VTK_LIBRARIES} QVTK libPointSetProcessing)
