import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers
import Example

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Qt Quick 3D Jolt Physics - Impeller")

    readonly property int nonMoving : 0
    readonly property int moving: 1
    readonly property int debris: 2
    readonly property int sensor: 3

    PhysicsSystem {
        id: physicsSystem
        scene: viewport.scene
        running: true
        gravity: Qt.vector3d(0, -490, 0)
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
        contactListener: ExampleSensorContactListener {}
    }

    View3D {
        id: viewport
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#d6dbdf"
            backgroundMode: SceneEnvironment.Color
        }

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 200, 1000)
            clipFar: 5000
            clipNear: 1
        }

        DirectionalLight {
            eulerRotation.x: -45
            eulerRotation.y: 45
            castsShadow: true
            brightness: 1
            shadowFactor: 100
        }

        Body {
            position: Qt.vector3d(0, -100, 0)
            eulerRotation: Qt.vector3d(-90, 0, 0)
            shape: PlaneShape {
                extent: 1000
            }
            objectLayer: nonMoving
            motionType: Body.Static
            Model {
                source: "#Rectangle"
                scale: Qt.vector3d(500, 500, 1)
                materials: DefaultMaterial {
                    diffuseColor: "green"
                }
                castsShadows: false
                receivesShadows: true
            }
        }

        Body {
            id: sphere
            density: 1
            position: Qt.vector3d(0, 600, 0)
            objectLayer: moving
            motionType: Body.Dynamic
            restitution: 0.5
            maxLinearVelocity: 1000

            property bool inArea: false

            shape: SphereShape {
                diameter: 100
            }

            onBodyEntered: (body) => {
                inArea = true;
            }

            onBodyExited: (body) => {
                inArea = false;
            }

            Model {
                source: "#Sphere"
                materials: PrincipledMaterial {
                    baseColor: sphere.inArea ? "yellow" : "red"
                }
            }
        }

        Body {
            position: Qt.vector3d(0, 350, 0)
            scale: Qt.vector3d(1, 2, 1)

            objectLayer: sensor
            motionType: Body.Static
            isSensor: true

            shape: BoxShape {
                id: boxShape
                extents: Qt.vector3d(100, 100, 100)
            }

            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColor: Qt.rgba(1, 0, 1, 0.2)
                    alphaMode: PrincipledMaterial.Blend
                }
            }
        }

        Body {
            position: Qt.vector3d(0, 0, 0)
            scale: Qt.vector3d(2, 2, 2)

            objectLayer: nonMoving
            motionType: Body.Static

            shape: SphereShape {
                diameter: 100
            }

            onBodyContact: (body) => {
               let velocity = Qt.vector3d(0, -1, 0).times(-700);
               body.setLinearVelocity(velocity);
            }

            Model {
                source: "#Sphere"
                materials: PrincipledMaterial {
                    baseColor: "blue"
                }
            }
        }
    }
}
