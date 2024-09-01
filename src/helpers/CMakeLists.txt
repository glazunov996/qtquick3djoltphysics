qt_internal_add_qml_module(Quick3DJoltPhysicsHelpers
    URI "QtQuick3D.JoltPhysics.Helpers"
    VERSION "${PROJECT_VERSION}"
    CLASS_NAME QtQuick3DPJolthysicsHelpersPlugin
    PLUGIN_TARGET qtquick3djoltphysicshelpersplugin
    SOURCES
        capsulegeometry.cpp capsulegeometry_p.h
    DEPENDENCIES
        QtQuick3D.JoltPhysics/auto
        QtQuick3D/auto
        QtQuick/auto
    PAST_MAJOR_VERSIONS 1
    DEFINES
    LIBRARIES
        Qt::Quick3DJoltPhysics
    PUBLIC_LIBRARIES
        Qt::Qml
        Qt::Quick
        Qt::Quick3D
)
