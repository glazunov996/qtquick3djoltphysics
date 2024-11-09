import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers
import Example

import ".."

Item {
    id: mainWindow

    anchors.fill: parent

    readonly property int nonMoving : 0
    readonly property int moving: 1
    readonly property int debris: 2
    readonly property int sensor: 3

    PhysicsSystem {
        id: physicsSystem
        scene: viewport.scene
        running: AppSettings.playPhysics
        gravity: Qt.vector3d(0, -981, 0)
        numThreads: AppSettings.maxConcurrentJobs
        collisionSteps: AppSettings.collisionSteps
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
        settings: PhysicsSettings {
            numVelocitySteps: Math.round(AppSettings.numVelocitySteps)
            numPositionSteps: Math.round(AppSettings.numPositionSteps)
            baumgarte: AppSettings.baumgarte
            speculativeContactDistance: AppSettings.speculativeContactDistance
            penetrationSlop: AppSettings.penetrationSlop
            linearCastThreshold: AppSettings.linearCastThreshold
            minVelocityForRestitution: AppSettings.minVelocityForRestitution
            timeBeforeSleep: AppSettings.timeBeforeSleep
            pointVelocitySleepThreshold: AppSettings.pointVelocitySleepThreshold
            deterministicSimulation: AppSettings.deterministicSimulation
            constraintWarmStart: AppSettings.constraintWarmStart
            useBodyPairContactCache: AppSettings.useBodyPairContactCache
            useManifoldReduction: AppSettings.useManifoldReduction
            useLargeIslandSplitter: AppSettings.useLargeIslandSplitter
            allowSleeping: AppSettings.allowSleeping
            checkActiveEdges: AppSettings.checkActiveEdges
        }
    }

    SampleCameraController {
        camera: camera
        physicsSystem: physicsSystem
        speed: 1
        shiftSpeed: 3
    }

    View3D {
        id: viewport
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#191970"
            backgroundMode: SceneEnvironment.Color
        }

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(-200, 100, 500)
            eulerRotation: Qt.vector3d(-20, -20, 0)
            clipFar: 50000
            clipNear: 1
        }

        DirectionalLight {
            eulerRotation.x: -45
            eulerRotation.y: 45
            castsShadow: true
            brightness: 1
            shadowFactor: 50
        }

        Body {
            position: Qt.vector3d(0, -100, 0)
            eulerRotation: Qt.vector3d(0, 0, 0)
            shape: PlaneShape {
                extent: 1000
            }
            objectLayer: nonMoving
            motionType: Body.Static
            Model {
                source: "#Rectangle"
                scale: Qt.vector3d(10, 10, 1)
                materials: DefaultMaterial {
                    diffuseColor: "green"
                }
                castsShadows: false
                receivesShadows: true
                eulerRotation.x: -90
            }
        }

        Body {
            position: Qt.vector3d(-100, 100, 0)
            shape: BoxShape {
                id: boxShape
                extents: Qt.vector3d(100, 100, 100)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            restitution: 0.5
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColor: "yellow"
                }
            }
        }

        Body {
            position: Qt.vector3d(0, 100, 0)
            shape: SphereShape {
                id: sphereShape
                diameter: 100
            }
            objectLayer: moving
            motionType: Body.Dynamic
            restitution: 0.5
            Model {
                source: "#Sphere"
                materials: PrincipledMaterial {
                    baseColor: "blue"
                }
            }
        }

        Body {
            position: Qt.vector3d(75, 200, 0)
            shape: CapsuleShape {
                id: capsuleShape
                diameter: 100
                height: 100
            }
            objectLayer: moving
            motionType: Body.Dynamic
            eulerRotation.z: 90
            restitution: 0.5
            Model {
                eulerRotation.z: -90
                geometry: CapsuleGeometry {}
                materials: PrincipledMaterial {
                    baseColor: "red"
                }
            }
        }
    }

    SettingsView {}
}
