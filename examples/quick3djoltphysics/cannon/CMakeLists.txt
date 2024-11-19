cmake_minimum_required(VERSION 3.16)
project(example_cannon LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/quick3dpjolthysics/${PROJECT_NAME}")

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

# Resources:
set(qml_resource_files
    "main.qml"
    "Box.qml"
    "Sphere.qml"
    "Crosshair.qml"
)

qt_add_resources(${PROJECT_NAME} "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
