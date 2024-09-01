pragma Singleton

import QtQuick
import QtQuick3D

QtObject {
    id: settings
    objectName: "test"
    // Antialiasing mode & quality used in all examples.
    property int antialiasingMode: SceneEnvironment.NoAA
    property int antialiasingQuality: SceneEnvironment.High
    // Toggle default visibility of these views
    property bool showSettingsView: true
    property bool showLoggingView: false
    // Fonts in pointSizes
    // These are used mostly on examples in 3D side
    property real fontSizeLarge: 16
    // These are used mostly on settings
    property real fontSizeSmall: 10
    property real iconSize: 24
    // Physics state
    property bool playPhysics: false

    signal testRestartNeeded();

    property int maxConcurrentJobs: 0
    property int gravity: 10
    property int collisionSteps: 1
    property int numVelocitySteps: 10
    property int numPositionSteps: 2
    property real baumgarte: 0.20
    property real speculativeContactDistance: 0.020
    property real penetrationSlop: 0.020
    property real linearCastThreshold: 0.75
    property real minVelocityForRestitution: 1.0
    property real timeBeforeSleep: 0.5
    property real pointVelocitySleepThreshold: 0.03
    property bool deterministicSimulation: false
    property bool constraintWarmStart: true
    property bool useBodyPairContactCache: true
    property bool useManifoldReduction: true
    property bool useLargeIslandSplitter: true
    property bool allowSleeping: true
    property bool checkActiveEdges: true
}
