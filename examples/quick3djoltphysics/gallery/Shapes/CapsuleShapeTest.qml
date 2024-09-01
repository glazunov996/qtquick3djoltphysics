import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
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
            shape: BoxShape {
                extents: Qt.vector3d(200, 2, 200)
            }
            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(0, -1, 0)
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: "white"
                    metalness: 0.5
                    roughness: 0.1
                }
                castsShadows: false
                receivesShadows: true
                scale: Qt.vector3d(2, 0.02, 2)
            }
        }

        Body {
            id: body1
            shape: CapsuleShape {
                height: 5
                diameter: 4
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(0, 10, 0)
            Model {
                eulerRotation.z: -90
                geometry: CapsuleGeometry {
                    height: 5
                    diameter: 4
                }
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    metalness: 0.5
                    roughness: 0.1
                    baseColor: colors[body1.bodyID % colors.length]
                }
            }
        }

        Body {
            id: body2
            shape: CapsuleShape {
                height: 5
                diameter: 4
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(10, 10, 0)
            eulerRotation.x: 90
            Model {
                eulerRotation.z: -90
                geometry: CapsuleGeometry {
                    height: 5
                    diameter: 4
                }
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    metalness: 0.5
                    roughness: 0.1
                    baseColor: colors[body2.bodyID % colors.length]
                }
            }
        }

        Component {
            id: capsuleComponent
            Body {
                id: capsuleBody
                shape: CapsuleShape {
                    height: 10
                    diameter: 2
                }
                objectLayer: moving
                motionType: Body.Dynamic
                Model {
                    eulerRotation.z: -90
                    geometry: CapsuleGeometry {
                        height: 10
                        diameter: 2
                    }
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        metalness: 0.0
                        roughness: 0.1
                        baseColor: colors[capsuleBody.bodyID % colors.length]
                    }
                }
            }
        }

        Component.onCompleted: {
            for (let i = 0; i < 10; ++i) {
                for (let j = 0; j < 2; ++j) {
                    let position;
                    let eulerRotation = Qt.vector3d(0, 0, 0)
                    if (i & 1) {
                        position = Qt.vector3d(-4 + 8 * j, 2 + 3 * i, -20);
                        eulerRotation.x = 90;
                    } else {
                        position = Qt.vector3d(0, 2 + 3 * i, -20 - 4 + 8 * j);
                        eulerRotation.z = 90
                    }
                    let capsule = capsuleComponent.createObject(viewport.scene,
                        { position: position, eulerRotation: eulerRotation })
                }
            }
        }
    }

    SettingsView {}
}