import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
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
            shape: BoxShape {
                extents: Qt.vector3d(6, 4, 3)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(-30, 10, 0)
            Model {
                source: "#Cube"
                receivesReflections: true
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
                scale: Qt.vector3d(0.06, 0.04, 0.03)
            }
        }


        Body {
            id: body2
            shape: BoxShape {
                extents: Qt.vector3d(6, 4, 3)
            }
            scale: Qt.vector3d(0.25, 0.25, 0.25)
            position: Qt.vector3d(-20, 10, 0)
            objectLayer: moving
            motionType: Body.Dynamic
            Model {
                source: "#Cube"
                receivesReflections: true
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
                scale: Qt.vector3d(0.06, 0.04, 0.03)
            }
        }

        Body {
            id: body3
            shape: BoxShape {
                extents: Qt.vector3d(6, 4, 3)
            }
            scale: Qt.vector3d(2, 2, 2)
            position: Qt.vector3d(-10, 10, 0)
            objectLayer: moving
            motionType: Body.Dynamic
            Model {
                source: "#Cube"
                receivesReflections: true
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[body3.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.06, 0.04, 0.03)
            }
        }

        Body {
            id: body4
            scale: Qt.vector3d(0.25, 0.5, 1.5)
            position: Qt.vector3d(0, 10, 0)
            shape: BoxShape {
                extents: Qt.vector3d(6, 4, 3)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            Model {
                source: "#Cube"
                receivesReflections: true
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[body4.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.06, 0.04, 0.03)
            }
        }

        Body {
            id: body5
            scale: Qt.vector3d(-0.25, 0.5, -1.5)
            position: Qt.vector3d(10, 10, 0)
            shape: BoxShape {
                extents: Qt.vector3d(6, 4, 3)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            Model {
                source: "#Cube"
                receivesReflections: true
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[body5.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.06, 0.04, 0.03)
            }
        }

        Body {
            id: body6
            scale: Qt.vector3d(-0.25, 0.5, 1.5)
            position: Qt.vector3d(20, 10, 0)
            shape: BoxShape {
                extents: Qt.vector3d(6, 4, 3)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            Model {
                source: "#Cube"
                receivesReflections: true
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[body6.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                    cullMode: Material.FrontFaceCulling
                }
                scale: Qt.vector3d(0.06, 0.04, 0.03)
            }
        }
    }

    SettingsView {}
}
