set(PHYSICS_REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/)

qt_internal_add_3rdparty_library(BundledJoltPhysics
    STATIC
    INSTALL
    LIBRARIES
        ${CMAKE_DL_LIBS}
    DEFINES
        QT_BUILD_JOLTPHYSICSSTATIC_LIB
)

include(Jolt/Jolt.cmake)

qt_internal_extend_target(BundledJoltPhysics
    SOURCES ${JOLT_PHYSICS_SRC_FILES}
    INCLUDE_DIRECTORIES
        ./
)

# Silence warnings
qt_disable_warnings(BundledJoltPhysics)
qt_set_symbol_visibility_hidden(BundledJoltPhysics)
