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

    property real time: 0.0

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
        onBeforeFrameDone: (deltaTime) => prePhysicsUpdate(deltaTime)
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

    function prePhysicsUpdate(frameDelta) {
        time += frameDelta;
        if (time >= 1)
            sphereBody.motionQuality = Body.Discrete;
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
            id: sphereBody
            shape: SphereShape {
                diameter: 2
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(0, 0.5, 0)
            friction: 0
            restitution: 1
            motionQuality: Body.LinearCast
            Model {
                source: "#Sphere"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[sphereBody.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.02, 0.02, 0.02)
            }
            Component.onCompleted: {
                sphereBody.setLinearVelocity(Qt.vector3d(-240, 0, -120))
            }
        }

        Body {
            id: compoundShapeBody
            shape: StaticCompoundShape {
                shapes: [
                    BoxShape {
                        extents: Qt.vector3d(10, 2, 0.2)
                        position.z: 5
                    },
                    BoxShape {
                        extents: Qt.vector3d(10, 2, 0.2)
                        position.z: -5
                    },
                    BoxShape {
                        extents: Qt.vector3d(10, 2, 0.2)
                        position.x: 5
                        eulerRotation.y: 90
                    },
                    BoxShape {
                        extents: Qt.vector3d(10, 2, 0.2)
                        position.x: -5
                        eulerRotation.y: 90
                    }
                ]
            }
            objectLayer: moving
            motionType: Body.Kinematic
            position: Qt.vector3d(0, 1, 0)
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[compoundShapeBody.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                position.z: 5
                scale: Qt.vector3d(0.1, 0.02, 0.002)
            }
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[compoundShapeBody.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                position.z: -5
                scale: Qt.vector3d(0.1, 0.02, 0.002)
            }
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[compoundShapeBody.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                position.x: 5
                eulerRotation.y: 90
                scale: Qt.vector3d(0.1, 0.02, 0.002)
            }
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[compoundShapeBody.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                position.x: -5
                eulerRotation.y: 90
                scale: Qt.vector3d(0.1, 0.02, 0.002)
            }
        }
    }

    SettingsView {}
}
