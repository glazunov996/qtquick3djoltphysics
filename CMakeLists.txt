cmake_minimum_required(VERSION 3.16)

include(.cmake.conf)
project(QtQuick3DJoltPhysics
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt Quick3D Jolt Physics Extensions"
    HOMEPAGE_URL "https://qt.io/"
    LANGUAGES CXX C
)

# Make sure we only use latest private CMake API, aka no compatibility wrappers.
set(QT_NO_INTERNAL_COMPATIBILITY_FUNCTIONS TRUE)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core)
find_package(Qt6 ${PROJECT_VERSION} CONFIG OPTIONAL_COMPONENTS Gui Qml Quick Quick3D QUIET)

qt_internal_project_setup()

qt_build_repo()