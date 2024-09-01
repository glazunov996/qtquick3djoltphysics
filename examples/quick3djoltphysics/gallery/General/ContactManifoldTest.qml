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

        Component {
            id: boxComponent
            Body {
                id: boxBody
                shape: BoxShape {
                    extents: Qt.vector3d(8, 8, 8)
                }
                objectLayer: nonMoving
                motionType: Body.Static
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
                    scale: Qt.vector3d(0.08, 0.08, 0.08)
                }
            }
        }

        Component {
            id: capsuleComponent
            Body {
                id: capsuleBody
                shape: CapsuleShape {
                    height: 10
                    diameter: 4
                }
                objectLayer: moving
                motionType: Body.Dynamic
                Model {
                    eulerRotation.z: -90
                    geometry: CapsuleGeometry {
                        height: 10
                        diameter: 4
                    }
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[capsuleBody.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
            }
        }

        Component {
            id: longBoxComponent
            Body {
                id: longBoxBody
                shape: BoxShape {
                    extents: Qt.vector3d(4, 14, 4)
                }
                objectLayer: moving
                motionType: Body.Dynamic
                Model {
                    source: "#Cube"
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[longBoxBody.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                    scale: Qt.vector3d(0.04, 0.14, 0.04)
                }
            }
        }

        Component.onCompleted: {
            for (var i = 0; i < 3; ++i) {
                for (var j = 0; j < 2; ++j) {
                    var position = Qt.vector3d(-20 + i * 10.0, 4, -20 + j * 40)
                    var box = boxComponent.createObject(physicsSystem.scene, { position: position })

                    var eulerRotation = Qt.vector3d(90, 18, 0)
                    if (j === 0) {
                        position = Qt.vector3d(-20.0 + i * 10, 12, -5 + i * 5 - 20 + j * 40)
                        box = capsuleComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: eulerRotation })
                    } else {
                        position = Qt.vector3d(-20.0 + i * 10, 12, -5 + i * 5 - 20 + j * 40)
                        box = longBoxComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: eulerRotation })
                    }
                }
            }
        }
    }

    SettingsView {}
}
