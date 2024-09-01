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

        GroupFilterTable {
            id: groupFilter
            numSubGroups: 15
            Component.onCompleted: {
                for (let i = 0; i < numSubGroups - 1; ++i)
                    disableCollision(i, i + 1);
            }
        }

        Body {
            id: top
            shape: CapsuleShape {
                height: 5
                diameter: 2
            }
            collisionGroup: CollisionGroup {
                groupFilterTable: groupFilter
                groupId: 0
                subGroupId: 0
            }

            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(0, 50, 0)
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

        Component {
            id: bodyComponent
            Body {
                id: body
                property int index: 0
                shape: CapsuleShape {
                    height: 5
                    diameter: 2
                }
                collisionGroup: CollisionGroup {
                    groupFilterTable: groupFilter
                    groupId: 0
                    subGroupId: index
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
            PointConstraint {}
        }

        Component.onCompleted: {
            let prev = top;
            let position = Qt.vector3d(0, 50, 0);
            for (let i = 1; i < 15; ++i) {
                position = position.plus(Qt.vector3d(2.0 * 2.5, 0, 0))
                let body = bodyComponent.createObject(viewport.scene, {index: i, position: position});

                let point = position.plus(Qt.vector3d(-2.5, 0, 0));
                let constraint = constraintComponent.createObject(viewport.scene, { point1: point, point2: point, body1: prev, body2: body });

                prev = body;
            }
        }
    }

    SettingsView {}
}
