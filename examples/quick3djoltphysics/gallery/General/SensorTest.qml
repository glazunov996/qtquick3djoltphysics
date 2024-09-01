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

    property real time: 0

    property alias sensorIDs: contactListener.sensorIDs
    property var bodies: []

    readonly property real worldScale: 0.2

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
        contactListener: ExampleSensorContactListener {
            id: contactListener
        }
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

    function prePhysicsUpdate(deltaTime) {
        time += deltaTime / 1000
        var pos = Qt.vector3d(-20 * Math.cos(time), 10, 0)
        kinematicBody.moveKinematic(pos, Qt.quaternion(1, 0, 0, 0), deltaTime / 1000)

        var center = Qt.vector3d(0, 10, 0)
        var centrifugalForce = 10.0
        var gravity = physicsSystem.gravity

        var bodiesInSensor = contactListener.getBodiesInSensor(0);
        for (var i = 0; i < bodiesInSensor.length; ++i) {
            var bodyID = bodiesInSensor[i];
            var body = null;
            for (var j = 0; j < bodies.length; ++j) {
                if (bodies[j].bodyID === bodyID) {
                    body = bodies[j];
                    break;
                }
            }
            if (body === null || body.motionType === Body.Kinematic)
                continue;

            var acceleration = center.minus(body.position)
            var length = acceleration.length();
            if (length > 0.0)
                acceleration = acceleration.times(centrifugalForce / length)
            else
                acceleration = Qt.vector3d(0, 0, 0)

            acceleration = acceleration.minus(gravity)

            body.addForce(acceleration.times(1 / body.getInverseMass()))
        }
    }

    SampleCameraController {
        camera: camera
        physicsSystem: physicsSystem
        speed: worldScale
        shiftSpeed: worldScale * 3
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
            position: Qt.vector3d(30, 10, 30).times(worldScale)
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
                extents: Qt.vector3d(400 * worldScale, 2, 400 * worldScale)
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
                scale: Qt.vector3d(4 * worldScale, 0.02, 4 * worldScale)
            }
        }

        // A static sensor that attracts dynamic bodies that enter its area
        Sensor {
            id: sensor1
            shape: SphereShape {
                diameter: 20
            }
            objectLayer: sensor
            motionType: Body.Static
            position: Qt.vector3d(0, 10, 0)
            Model {
                source: "#Sphere"
                materials: DefaultMaterial {
                    diffuseColor: "white"
                    opacity: 0.1
                }
                scale: Qt.vector3d(0.2, 0.2, 0.2)
                castsShadows: false
            }
            onBodyIDChanged: sensorIDs.push(sensor1.bodyID)
        }

        // A static sensor that only detects active bodies
        Sensor {
            id: sensor2
            shape: BoxShape {
                extents: Qt.vector3d(10, 10, 10)
            }
            objectLayer: sensor
            motionType: Body.Static
            position: Qt.vector3d(-10, 5.1, 0)
            Model {
                source: "#Cube"
                materials: DefaultMaterial {
                    diffuseColor: "white"
                    opacity: 0.1
                }
                scale: Qt.vector3d(0.1, 0.1, 0.1)
                castsShadows: false
            }
            onBodyIDChanged: sensorIDs.push(sensor2.bodyID)
        }

        // Dynamic bodies
        Repeater3D {
            model: 15
            Body {
                id: body
                shape: BoxShape {
                    extents: Qt.vector3d(0.2, 1, 0.4)
                }
                objectLayer: moving
                motionType: Body.Dynamic
                position: Qt.vector3d(-15 + index * 3, 25, 0)
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
                    scale: Qt.vector3d(0.002, 0.01, 0.004)
                }
                onBodyIDChanged: bodies.push(body)
            }
        }

        Body {
            id: kinematicBody
            shape: BoxShape {
                extents: Qt.vector3d(0.5, 1, 2)
            }
            objectLayer: moving
            motionType: Body.Kinematic
            position: Qt.vector3d(-20, 10, 0)
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[kinematicBody.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.005, 0.01, 0.02)
            }
        }
    }

    SettingsView {}
}
