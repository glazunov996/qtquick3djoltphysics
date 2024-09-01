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

    property int currentShapeIndex: 0

    property var shapeComponents: []
    property var modelCompoents: []

    property var model: null
    property var shape: null

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
        if (time >= 3000) {
            var index = Math.floor(time / 3000) % shapeComponents.length;
            if (currentShapeIndex !== index) {
                currentShapeIndex = index;
                shape.destroy();
                model.destroy();
                shape = shapeComponents[index].createObject(null);
                model = modelCompoents[index].createObject(null)
                body.shape = shape;
                body.children = model;
            }
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
            id: boxShapeComponent
            BoxShape {
                extents: Qt.vector3d(1, 3, 1)
            }
        }

        Component {
            id: sphereShapeComponent
            SphereShape {
                diameter: 1
            }
        }

        Component {
            id: cylinderShapeComponent
            CylinderShape {
                height: 2
                diameter: 1
            }
        }

        Component {
            id: boxComponent
            Model {
                id: box
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[1]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.01, 0.03, 0.01)
            }
        }

        Component {
            id: sphereComponent
            Model {
                source: "#Sphere"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[1]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.01, 0.01, 0.01)
            }
        }

        Component {
            id: cylinderComponent
            Model {
                source: "#Cylinder"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[1]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.01, 0.02, 0.01)
            }
        }

        Body {
            id: body
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(0, 10, 0)
        }

        Component.onCompleted: {
            shapeComponents.push(boxShapeComponent)
            shapeComponents.push(sphereShapeComponent)
            shapeComponents.push(cylinderShapeComponent)

            modelCompoents.push(boxComponent)
            modelCompoents.push(sphereComponent)
            modelCompoents.push(cylinderComponent)

            shape = shapeComponents[0].createObject(null);
            model = modelCompoents[0].createObject(null)
            body.shape = shape;
            body.children = model;
        }

        Component.onDestruction: {
            shape.destroy();
            model.destroy();
        }
    }

    SettingsView {}
}
