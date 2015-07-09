ENABLE_TESTING()

# Normal orientation to point
ADD_EXECUTABLE(TestPointSetNormalOrientationToPoint TestPointSetNormalOrientationToPoint.cxx ${PointSetProcessingDir}/vtkPointSetNormalOrientationToPoint.cxx)
TARGET_LINK_LIBRARIES(TestPointSetNormalOrientationToPoint ${VTK_LIBRARIES} )
ADD_TEST(TestPointSetNormalOrientationToPoint TestPointSetNormalOrientationToPoint)

# Outlier Removal
ADD_EXECUTABLE(TestPointSetOutlierRemoval TestPointSetOutlierRemoval.cxx ${PointSetProcessingDir}/vtkPointSetOutlierRemoval.cxx)
TARGET_LINK_LIBRARIES(TestPointSetOutlierRemoval ${VTK_LIBRARIES} )
ADD_TEST(TestPointSetOutlierRemoval TestPointSetOutlierRemoval ${DataDir}/vtkPointSetOutlierRemoval_Input.vtp ${DataDir}/vtkPointSetOutlierRemoval_GroundTruth.vtp)

# Curvature Estimation
ADD_EXECUTABLE(TestPointSetCurvatureEstimation TestPointSetCurvatureEstimation.cxx 
${PointSetProcessingDir}/vtkPointSetNormalEstimation.cxx
${PointSetProcessingDir}/vtkPointSetCurvatureEstimation.cxx
)
TARGET_LINK_LIBRARIES(TestPointSetCurvatureEstimation ${VTK_LIBRARIES} )
ADD_TEST(TestPointSetCurvatureEstimation TestPointSetCurvatureEstimation ${DataDir}/vtkPointSetCurvatureEstimation_Input.vtp ${DataDir}/vtkPointSetCurvatureEstimation_GroundTruth.vtp)

# Normal Estimation
ADD_EXECUTABLE(TestPointSetNormalEstimation TestPointSetNormalEstimation.cxx 
${PointSetProcessingDir}/vtkPointSetNormalEstimation.cxx)
TARGET_LINK_LIBRARIES(TestPointSetNormalEstimation ${VTK_LIBRARIES} )
ADD_TEST(TestPointSetNormalEstimation TestPointSetNormalEstimation ${DataDir}/vtkPointSetNormalEstimation_Input.vtp ${DataDir}/vtkPointSetNormalEstimation_GroundTruth.vtp)

# Normal Orientation
ADD_EXECUTABLE(TestPointSetNormalOrientation TestPointSetNormalOrientation.cxx 
${PointSetProcessingDir}/vtkPointSetNormalOrientation.cxx
${PointSetProcessingDir}/vtkEuclideanMinimumSpanningTree.cxx  
${PointSetProcessingDir}/vtkRiemannianGraphFilter.cxx
${PointSetProcessingDir}/vtkKNNGraphFilter.cxx)
TARGET_LINK_LIBRARIES(TestPointSetNormalOrientation ${VTK_LIBRARIES})
ADD_TEST(TestPointSetNormalOrientation TestPointSetNormalOrientation ${DataDir}/vtkPointSetNormalOrientation_Input.vtp ${DataDir}/vtkPointSetNormalOrientation_GroundTruth.vtp)
