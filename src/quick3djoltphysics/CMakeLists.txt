#####################################################################
## Quick3DJoltPhysics Module:
#####################################################################

qt_internal_add_qml_module(Quick3DJoltPhysics
    URI "QtQuick3D.JoltPhysics"
        VERSION "${PROJECT_VERSION}"
        CLASS_NAME QtQuick3DJoltPhysicsPlugin
        PLUGIN_TARGET qquick3djoltphysicsplugin
        DESIGNER_SUPPORTED
        NO_PLUGIN_OPTIONAL
        NO_GENERATE_PLUGIN_SOURCE
    SOURCES
        abstractphysicsconstraint_p.h abstractphysicsconstraint.cpp
        abstractphysicsbody_p.h abstractphysicsbody.cpp
        abstractphysicsnode_p.h abstractphysicsnode.cpp
        abstractshape_p.h abstractshape.cpp
        body_p.h body.cpp
        boxshape_p.h boxshape.cpp
        capsuleshape_p.h capsuleshape.cpp
        charactercontactlistener.h charactercontactlistener.cpp
        character_p.h character.cpp
        charactervirtual_p.h charactervirtual.cpp
        collisiongroup_p.h collisiongroup.cpp
        contactlistener.h contactlistener.cpp
        convexhullshape_p.h convexhullshape.cpp
        cylindershape_p.h cylindershape.cpp
        distanceconstraint_p.h distanceconstraint.cpp
        fixedconstraint_p.h fixedconstraint.cpp
        groupfiltertable_p.h groupfiltertable.cpp
        hingeconstraint_p.h hingeconstraint.cpp
        meshshape_p.h meshshape.cpp
        physicslayers.h physicslayers.cpp
        physicsmeshutils_p_p.h
        physicssettings_p.h physicssettings.cpp
        physicssystem_p.h physicssystem.cpp
        physicsutils_p.h
        pointconstraint_p.h pointconstraint.cpp
        qtquick3djoltphysicsglobal.h
        qtquick3djoltphysicsglobal_p.h
        raycastresult_p.h raycastresult.cpp
        sensor_p.h sensor.cpp
        sphereshape_p.h sphereshape.cpp
        springsettings_p.h springsettings.cpp
        staticcompoundshape_p.h staticcompoundshape.cpp
        swingtwistconstraint_p.h swingtwistconstraint.cpp
    DEFINES
    SYSTEM_INCLUDE_DIRECTORIES
        ../3rdparty/JoltPhysics/
    LIBRARIES
        Qt::CorePrivate
        Qt::GuiPrivate
        Qt::QmlPrivate
        Qt::Quick3DPrivate
        Qt::QuickPrivate
        Qt::BundledJoltPhysics
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::Quick3D
    PRIVATE_MODULE_INTERFACE
        Qt::CorePrivate
        Qt::GuiPrivate
        Qt::QmlPrivate
        Qt::Quick3D
        Qt::QuickPrivate
)

qt_internal_extend_target(qquick3djoltphysicsplugin
    SOURCES
        plugin.cpp
    LIBRARIES
        Qt::Quick3DJoltPhysicsPrivate
)
