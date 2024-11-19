cmake_minimum_required(VERSION 3.16)
project(example_gallery LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/quick3djoltphysics/${PROJECT_NAME}")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Quick3D Quick3DJoltPhysics)

qt_add_executable(${PROJECT_NAME}
    main.cpp
    ../shared/examplecharactercontactlistener.cpp
    ../shared/examplecharactercontactlistener.h
    ../shared/examplecontactlistener.cpp
    ../shared/examplecontactlistener.h
    ../shared/examplegeometry.cpp
    ../shared/examplegeometry.h
    ../shared/examplelayers.cpp
    ../shared/examplelayers.h
    ../shared/examplesensorcontactlistener.cpp
    ../shared/examplesensorcontactlistener.h
    ../shared/shared.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Quick3D
    Qt::Quick3DJoltPhysics
)

set_source_files_properties(AppSettings.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(${PROJECT_NAME}
    URI Example
    VERSION 1.0
    QML_FILES
        main.qml
    SOURCES
        mousehandler.h mousehandler.cpp
        RESOURCES
            images/arrow_icon.png
            images/checkers2.png
            images/heightmap1.png
            images/icon_pause.png
            images/icon_play.png
            images/icon_settings.png
            images/qt_logo.png
            meshes/Cube.mesh
    NO_RESOURCE_TARGET_PATH
    QML_FILES AppSettings.qml
    QML_FILES Character/CharacterVirtualTest.qml
    QML_FILES Constraints/DistanceConstraintTest.qml
    QML_FILES Constraints/HingeConstraintTest.qml
    QML_FILES Constraints/PointConstraintTest.qml
    QML_FILES General/ChangeMotionQualityTest.qml
    QML_FILES General/ChangeObjectLayerTest.qml
    QML_FILES General/ChangeShapeTest.qml
    QML_FILES General/ContactListenerTest.qml
    QML_FILES General/ContactManifoldTest.qml
    QML_FILES General/DampingTest.qml
    QML_FILES General/FrictionTest.qml
    QML_FILES General/GravityFactorTest.qml
    QML_FILES General/HighSpeedTest.qml
    QML_FILES General/KinematicTest.qml
    QML_FILES General/PyramidTest.qml
    QML_FILES General/RestitutionTest.qml
    QML_FILES General/SensorTest.qml
    QML_FILES General/SimpleTest.qml
    QML_FILES General/StackTest.qml
    QML_FILES General/TwoDFunnelTest.qml
    QML_FILES General/WallTest.qml
    QML_FILES SampleCharacterCameraController.qml
    QML_FILES SampleCameraController.qml
    QML_FILES ScaledShapes/ScaledBoxShapeTest.qml
    QML_FILES ScaledShapes/ScaledCapsuleShapeTest.qml
    QML_FILES ScaledShapes/ScaledCylinderShapeTest.qml
    QML_FILES ScaledShapes/ScaledSphereShapeTest.qml
    QML_FILES SettingsView.qml
    QML_FILES Shapes/BoxShapeTest.qml
    QML_FILES Shapes/CapsuleShapeTest.qml
    QML_FILES Shapes/ConvexHullShapeTest.qml
    QML_FILES Shapes/CylinderShapeTest.qml
    QML_FILES Shapes/HeightFieldShapeTest.qml
    QML_FILES Shapes/MeshShapeTest.qml
    QML_FILES Shapes/OffsetCenterOfMassShapeTest.qml
    QML_FILES Shapes/SphereShapeTest.qml
    QML_FILES Shapes/StaticCompoundShapeTest.qml
    QML_FILES Shapes/PlaneShapeTest.qml
    QML_FILES StartupView.qml
    QML_FILES Character/CharacterTest.qml
    QML_FILES General/CenterOfMassTest.qml
    QML_FILES CustomLabel.qml
    QML_FILES CustomCheckBox.qml
    QML_FILES CustomSlider.qml
    QML_FILES Constraints/SwingTwistConstraintTest.qml
    QML_FILES Constraints/FixedConstraintTest.qml
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
