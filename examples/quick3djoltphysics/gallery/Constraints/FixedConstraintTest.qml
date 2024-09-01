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

    function getRandomFloat(min, max) {
        return Math.random() * (max - min) + min;
    }

    function sinCos(angle) {
        return { sin: Math.sin(angle), cos: Math.cos(angle) };
    }

    function getRandomQuat() {
        const x0 = getRandomFloat(0.0, 1.0);
        const r1 = Math.sqrt(1.0 - x0);
        const r2 = Math.sqrt(x0);
        const angle1 = getRandomFloat(0.0, 2.0 * Math.PI);
        const angle2 = getRandomFloat(0.0, 2.0 * Math.PI);

        const sinCos1 = sinCos(angle1);
        const sinCos2 = sinCos(angle2);

        return Qt.quaternion(
            sinCos2.cos * r2,
            sinCos1.sin * r1,
            sinCos1.cos * r1,
            sinCos2.sin * r2)
    }

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

        Body {
            id: top
            shape: BoxShape {
                extents: Qt.vector3d(4, 4, 4)
            }
            collisionGroup: CollisionGroup {
                groupFilterTable: groupFilter
                groupId: 0
                subGroupId: 0
            }
            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(0, 25, 0)
            Model {
                source: "#Cube"
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
                scale: Qt.vector3d(0.04, 0.04, 0.04)
            }
        }

        Body {
            id: top2
            shape: BoxShape {
                extents: Qt.vector3d(4, 4, 4)
            }
            collisionGroup: CollisionGroup {
                groupFilterTable: groupFilter
                groupId: 1
                subGroupId: 0
            }
            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(0, 25, -20)
            Model {
                source: "#Cube"
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
                scale: Qt.vector3d(0.04, 0.04, 0.04)
            }
        }

        Component {
            id: bodyComponent
            Body {
                id: body
                property int index: 0
                property int groupId: 0
                shape: BoxShape {
                    extents: Qt.vector3d(4, 4, 4)
                }
                collisionGroup: CollisionGroup {
                    groupFilterTable: groupFilter
                    groupId: groupId
                    subGroupId: index
                }
                objectLayer: moving
                motionType: Body.Dynamic
                Model {
                    id: model
                    source: "#Cube"
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
                    scale: Qt.vector3d(0.04, 0.04, 0.04)
                }
            }
        }

        Component {
            id: constraintComponent
            FixedConstraint {}
        }

        function initBodies(variation) {
            let prev = variation === 0 ? top : top2;
            let position = Qt.vector3d(0, 25, -20 * variation);
            for (let i = 1; i < 10; ++i) {
                let rotation;
                if (variation === 0) {
                    position = position.plus(Qt.vector3d(4, 0, 0))
                    rotation = Qt.quaternion(1, 0, 0, 0)
                } else {
                    position = position.plus(Qt.vector3d(4 + Math.abs(getRandomFloat(-1, 1)), getRandomFloat(-1, 1), getRandomFloat(-1, 1)))
                    rotation = getRandomQuat();
                }

                let body = bodyComponent.createObject(viewport.scene, {index: i, groupId: variation, position: position, rotation: rotation});

                let constraint;
                if ((i & 1) == 0) {
                    let point = position.plus(Qt.vector3d(-0.5 * 4, 0, 0.5 * 4));
                    constraint = constraintComponent.createObject(viewport.scene, {
                            point1: point,
                            point2: point,
                            body1: prev,
                            body2: body,
                    });
                } else {
                    let point = position.plus(Qt.vector3d(-0.5 * 4, 0, -0.5 * 4));
                    constraint = constraintComponent.createObject(viewport.scene, {
                            point1: point,
                            point2: point,
                            body1: prev,
                            body2: body,
                    });
                }

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
