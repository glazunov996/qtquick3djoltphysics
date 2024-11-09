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
    property bool isDebris: false
    property real time: 0.0

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

    function prePhysicsUpdate(frameDelta) {
        time += frameDelta;
        if (time >= 2) {
            isDebris = !isDebris;
            movingBody.position = Qt.vector3d(0, 1.5, 0)

            for (var i = 0; i < 50; ++i) {
                var body = bodies[i]
                var randomX = getRandomFloat(-7.5, 7.5)
                var randomY = getRandomFloat(-7.5, 7.5)
                var rotation = getRandomQuat()
                body.position = Qt.vector3d(randomX, 2.0, randomY)
                body.rotation = rotation
                body.objectLayer = isDebris ? debris : moving
            }

            time = 0.0
        }
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
            id: movingBody
            shape: BoxShape {
                extents: Qt.vector3d(10, 0.2, 10)
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(0, 1.5, 0)
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[movingBody.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.1, 0.002, 0.1)
            }
        }

        Component {
            id: boxComponent
            Body {
                id: body
                shape: BoxShape {
                    extents: Qt.vector3d(0.2, 0.2, 0.2)
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
                    scale: Qt.vector3d(0.002, 0.002, 0.002)
                }
            }
        }

        Component.onCompleted: {
            for (var i = 0; i < 50; ++i) {
                var randomX = getRandomFloat(-10, 10)
                var randomY = getRandomFloat(-10, 10)
                var position = Qt.vector3d(randomX, 2.0, randomY)
                var rotation = getRandomQuat()
                var body = boxComponent.createObject(physicsSystem.scene, { position: position, rotation: rotation })
                bodies.push(body)
            }
        }
    }

    SettingsView {}
}
