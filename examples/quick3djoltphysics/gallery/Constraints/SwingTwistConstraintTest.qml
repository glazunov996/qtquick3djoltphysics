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

    property var constraints: []

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
            numSubGroups: 10
            Component.onCompleted: {
                for (let i = 0; i < numSubGroups - 1; ++i)
                    disableCollision(i, i + 1);
            }
        }

        Component {
            id: bodyComponent
            Body {
                id: body
                property int index: 0
                shape: CapsuleShape {
                    height: 3
                    diameter: 1
                }
                collisionGroup: CollisionGroup {
                    groupFilterTable: groupFilter
                    groupId: 0
                    subGroupId: index
                }

                objectLayer: moving
                motionType: Body.Dynamic
                allowSleeping: false
                eulerRotation.z: 90
                Model {
                    id: model
                    eulerRotation.z: -90
                    geometry: CapsuleGeometry {
                        height: 3
                        diameter: 1
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
            SwingTwistConstraint {}
        }

        Component.onCompleted: {
            let prev = null;
            let rotation = Quaternion.fromAxisAndAngle(0, 0, 1, 90);
            let position = Qt.vector3d(0, 25, 0);
            for (let i = 0; i < 10; ++i) {
                position = position.plus(Qt.vector3d(2.0 * 1.5, 0, 0))
                let body = bodyComponent.createObject(viewport.scene, {
                                index: i,
                                position: position,
                                rotation: Quaternion.fromAxisAndAngle(1, 0, 0, 45 * i).times(rotation),
                                objectLayer: i === 0 ? nonMoving : moving,
                                motionType: i === 0 ? Body.Static : Body.Dynamic
                });

                if (prev !== null) {
                    let constraint = constraintComponent.createObject(viewport.scene, {
                                            position1: position.plus(Qt.vector3d(-1.5, 0, 0)),
                                            position2: position.plus(Qt.vector3d(-1.5, 0, 0)),
                                            body1: prev,
                                            body2: body,
                                            twistAxis1: Qt.vector3d(1, 0, 0),
                                            twistAxis2: Qt.vector3d(1, 0, 0),
                                            planeAxis1: Qt.vector3d(0, 1, 0),
                                            planeAxis2: Qt.vector3d(0, 1, 0),
                                            normalHalfConeAngle: 60,
                                            planeHalfConeAngle: 20,
                                            twistMinAngle: -10,
                                            twistMaxAngle: 20,
                    });
                    constraints.push(constraint)
                }
                prev = body;
            }
        }
    }

    SettingsView {}
}
