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
            id: top
            shape: CapsuleShape {
                height: 5
                diameter: 2
            }

            objectLayer: 0
            motionType: Body.Static
            position: Qt.vector3d(0, 75, 0)
            eulerRotation.z: 90
            Model {
                eulerRotation.z: -90
                geometry: CapsuleGeometry {
                    height: 5
                    diameter: 2
                }
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[top.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Body {
            id: top2
            shape: CapsuleShape {
                height: 5
                diameter: 2
            }

            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(0, 75, 10)
            eulerRotation.z: 90
            Model {
                eulerRotation.z: -90
                geometry: CapsuleGeometry {
                    height: 5
                    diameter: 2
                }
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[top2.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        Component {
            id: bodyComponent
            Body {
                id: body
                property int index: 0
                shape: CapsuleShape {
                    height: 5
                    diameter: 2
                }

                objectLayer: moving
                motionType: Body.Dynamic
                eulerRotation.z: 90
                Model {
                    id: model
                    eulerRotation.z: -90
                    geometry: CapsuleGeometry {
                        height: 5
                        diameter: 2
                    }
                    materials: PrincipledMaterial {
                        baseColorMap: Texture {
                            source: "qrc:/images/checkers2.png"
                            scaleU: 2
                            scaleV: 2
                        }
                        baseColor: colors[body.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                }
            }
        }

        Component {
            id: constraintComponent
            DistanceConstraint {}
        }

        function initBodies(variation) {
            let prev = variation === 0 ? top : top2;
            let position = Qt.vector3d(0, 75, 10 * variation);
            for (let i = 1; i < 15; ++i) {
                position = position.plus(Qt.vector3d(5.0 + 2.0 * 2.5, 0, 0))
                let body = bodyComponent.createObject(viewport.scene, {index: i, position: position});

                let minDistance = 5;
                let maxDistance = 5;
                if (variation === 1) {
                    minDistance = 4;
                    maxDistance = 8;
                }

                let point1 = position.plus(Qt.vector3d(-7.5, 0, 0));
                let point2 = position.plus(Qt.vector3d(-2.5, 0, 0));
                let constraint = constraintComponent.createObject(viewport.scene, {
                                                                      point1: point1, point2: point2,
                                                                      body1: prev, body2: body,
                                                                      minDistance: minDistance, maxDistance: maxDistance
                                                                  });
                prev = body;
            }
        }

        Component.onCompleted: {
            initBodies(0);
            initBodies(1);
        }
    }

    SettingsView {}
}
