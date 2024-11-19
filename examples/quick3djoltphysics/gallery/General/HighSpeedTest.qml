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

    property var bodies: []

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
            id: dominoBlockComponent
            Body {
                id: body
                shape: BoxShape {
                    id: boxShape
                    extents: Qt.vector3d(1.8, 2.0, 0.2)
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
                        baseColor: colors[body.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                    scale: Qt.vector3d(0.018, 0.02, 0.002)
                }
            }
        }

        Component {
            id: dynamicObjectComponent
            Body {
                objectLayer: moving
                motionType: Body.Dynamic
                friction: 0.0
                restitution: 1.0
                linearDamping: 0.0
                angularDamping: 0.0
            }
        }

        Component {
            id: sphereShapeComponent
            SphereShape {}
        }

        Component {
            id: boxShapeComponent
            BoxShape {}
        }

        Component {
            id: modelComponent
            Model {
                property int bodyID: 0
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
            }
        }

        function createDominoBlocks(offset, numWalls, density, radius) {
            for (var i = 0; i < numWalls; ++i) {
                var position = offset.plus(Qt.vector3d(2 * i, 1, -1.1 - radius));
                var body = dominoBlockComponent.createObject(physicsSystem.scene, { position: position });
                bodies.push(body);

                position = offset.plus(Qt.vector3d(2 * i, 1, 1.1 + radius));
                body = dominoBlockComponent.createObject(physicsSystem.scene, { position: position });
                bodies.push(body);
            }

            position = offset.plus(Qt.vector3d(-1.1 - radius, 1, 0));
            var rotation = Quaternion.fromAxisAndAngle(0, 1, 0, 90);
            body = dominoBlockComponent.createObject(physicsSystem.scene, { position: position, rotation: rotation });
            bodies.push(body);
        }

        function createDynamicObject(position, velocity, modelProps, shape, motionQuality = Body.LinearCast) {
            var body = dynamicObjectComponent.createObject(physicsSystem.scene, {
                                    position: position, shape: shape, motionQuality: motionQuality})
            body.setLinearVelocity(velocity)
            modelProps["bodyID"] = body.bodyID
            modelComponent.createObject(body, modelProps)
            bodies.push(body);
        }

        Component.onCompleted: {
            var offset = Qt.vector3d(0, 0, -30);
            offset = offset.plus(Qt.vector3d(0, 0, 5));

            {
                createDominoBlocks(offset, 5, 2000.0, 0.1);
                var shape = sphereShapeComponent.createObject(null, { diameter: 0.2 })
                createDynamicObject(
                        offset.plus(Qt.vector3d(2.0 * 5 - 1, 1, 0)),  Qt.vector3d(-240.0, 0, -240.0),
                        { source: "#Sphere", scale: Qt.vector3d(0.002, 0.002, 0.002) }, shape)
            }

            offset = offset.plus(Qt.vector3d(0, 0, 5));

            {
                createDominoBlocks(offset, 5, 2000.0, 0.1);
                shape = boxShapeComponent.createObject(null, { extents: Qt.vector3d(0.2, 0.2, 0.2) })
                createDynamicObject(
                        offset.plus(Qt.vector3d(2.0 * 5 - 1, 1, 0)), Qt.vector3d(-240.0, 0, -240.0),
                        { source: "#Cube", scale: Qt.vector3d(0.002, 0.002, 0.002) }, shape)
            }

            offset = offset.plus(Qt.vector3d(0, 0, 5));

            {
                shape = boxShapeComponent.createObject(null, { extents: Qt.vector3d(0.2, 0.2, 0.2) })
                createDynamicObject(
                        offset.plus(Qt.vector3d(-1, 1, 0)), Qt.vector3d(0, 240.0, 0),
                        { source: "#Cube", scale: Qt.vector3d(0.002, 0.002, 0.002) }, shape)

                shape = boxShapeComponent.createObject(null, { extents: Qt.vector3d(0.2, 0.2, 0.2) })
                createDynamicObject(
                        offset.plus(Qt.vector3d(1, 1, 0)), Qt.vector3d(0, -240.0, 0),
                        { source: "#Cube", scale: Qt.vector3d(0.002, 0.002, 0.002) }, shape)
            }
        }
    }

    SettingsView {}
}
