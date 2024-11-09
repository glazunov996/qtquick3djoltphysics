import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.JoltPhysics
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
        gravity: Qt.vector3d(0, -AppSettings.gravity, 0)
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
        speed: 0.1
        shiftSpeed: 0.3
    }

    Component.onCompleted: {
        const normal = Qt.vector3d(0.1, 1.0, 0);

        const yaw = Math.atan2(normal.x, normal.z);
        const pitch = Math.atan2(-normal.y, Math.sqrt(normal.x * normal.x + normal.z * normal.z));

        const x = pitch * (180 / Math.PI) + 90;
        const y = yaw * (180 / Math.PI) - 90;

        console.log(x, y)
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
            position: Qt.vector3d(30, 10, 30)
            clipFar: 50000
            clipNear: 1
            fieldOfView: 70

            Component.onCompleted: {
                camera.lookAt(Qt.vector3d(-30, -10, -30))
            }
        }

        DirectionalLight {
            eulerRotation.x: -45
            eulerRotation.y: 45
            brightness: 1.5
            castsShadow: true
            shadowFactor: 100
            shadowMapQuality: Light.ShadowMapQualityVeryHigh
        }

        Body {
            shape: PlaneShape {
                extent: 200
            }
            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(0, 0, 0)
            eulerRotation.z: -5.7
            Model {
                source: "#Rectangle"
                materials: PrincipledMaterial {
                    baseColor: "white"
                    metalness: 0.5
                    roughness: 0.1
                }
                castsShadows: false
                scale: Qt.vector3d(2, 2, 0.01)
                eulerRotation.x: -90
            }
        }

        Body {
            id: body1
            shape: BoxShape {
                extents: Qt.vector3d(1, 1, 1)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(2, 1, 0)
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[body1.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.01, 0.01, 0.01)
            }
        }

        Body {
            id: body2
            shape: SphereShape {
                diameter: 1
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(0, 1, 0)
            eulerRotation.x: 45
            Model {
                source: "#Sphere"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[body2.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.01, 0.01, 0.01)
            }
        }
    }

    SettingsView {}
}
