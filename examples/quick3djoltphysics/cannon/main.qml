import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers
import QtQuick.Controls
import QtQuick.Layouts
import Example

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Qt Quick 3D Jolt Physics - Cannon")

    readonly property int nonMoving : 0
    readonly property int moving: 1
    readonly property int debris: 2
    readonly property int sensor: 3

    PhysicsSystem {
        id: physicsSystem
        scene: viewport.scene
        running: true
        gravity: Qt.vector3d(0, -981, 0)
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
    }

    View3D {
        id: viewport
        anchors.fill: parent
        focus: true

        environment: SceneEnvironment {
            antialiasingMode: SceneEnvironment.MSAA
            backgroundMode: SceneEnvironment.Color
            clearColor: "#f0f0f0"
        }

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(-4000, 5000, 10000)
            eulerRotation: Qt.vector3d(-20, -20, 0)
            clipFar: 500000
            clipNear: 100
        }

        DirectionalLight {
            eulerRotation: Qt.vector3d(-45, 45, 0)
            castsShadow: true
            brightness: 1
            shadowMapQuality: Light.ShadowMapQualityVeryHigh
            shadowFactor: 100
        }

        Body {
            eulerRotation: Qt.vector3d(-90, 0, 0)
            shape: PlaneShape {
                extent: 200000
            }
            objectLayer: nonMoving
            motionType: Body.Static
            Model {
                source: "#Rectangle"
                scale: Qt.vector3d(2000, 2000, 1)
                materials: DefaultMaterial {
                    diffuseColor: "green"
                }
                castsShadows: false
                receivesShadows: true
            }
        }

        Node {
            id: shapeSpawner
            property var instancesBoxes: []
            property var instancesSpheres: []
            property int stackCount: 0
            property var boxComponent: Qt.createComponent("Box.qml")
            property var sphereComponent: Qt.createComponent("Sphere.qml")

            function createStack(stackZ, numStacks) {
                let size = 10
                let extents = 400

                for (var i = 0; i < size; i++) {
                    for (var j = 0; j < size - i; j++) {
                        let x = j * 2 - size + i
                        let y = i * 2 + 1
                        let z = 5 * (stackZ - numStacks)
                        let center = Qt.vector3d(x, y, z).times(0.5 * extents)
                        let box = boxComponent.incubateObject(shapeSpawner, {
                                                                  "position": center,
                                                                  "xyzExtents": extents,
                                                                  "maxLinearVelocity" : 80000,
                                                                  "gravityFactor": 2
                                                              })
                        instancesBoxes.push(box)
                    }
                }
            }

            function createBall(position, forward) {
                var diameter = 600
                var speed = 40000
                let settings = {
                    "position": position,
                    "sphereDiameter": diameter,
                    "maxLinearVelocity": speed * 2,
                }
                let sphere = sphereComponent.createObject(shapeSpawner, settings)
                sphere.setLinearVelocity(forward.times(speed))
                instancesSpheres.push(sphere)

                if (sphere === null) {
                    console.log("Error creating object")
                }
            }

            function reset() {
                // Only run method if previous stack has been created fully
                for (var i = 0; i < instancesBoxes.length; i++)
                    if (!instancesBoxes[i].object)
                        return

                instancesSpheres.forEach(sphere => { sphere.destroy() })
                instancesBoxes.forEach(box => { box.object.destroy() })
                instancesSpheres = []
                instancesBoxes = []

                for (var stackI = 0; stackI < stackSlider.value; stackI++) {
                    shapeSpawner.createStack(stackI, stackSlider.value)
                }
            }
        }

        Crosshair {
            id: crossHair
            anchors.centerIn: parent
        }
    }

    Component.onCompleted: {
        shapeSpawner.reset()
    }

    WasdController {
        speed: 100
        controlledObject: camera
        Keys.onPressed: event => {
                            handleKeyPress(event)
                            if (event.key === Qt.Key_Space) {
                                shapeSpawner.createBall(camera.position,
                                                        camera.forward)
                            }
                        }
        Keys.onReleased: event => {
                             handleKeyRelease(event)
                         }
    }

    Frame {
        background: Rectangle {
            color: "#c0c0c0"
            border.color: "#202020"
        }
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10

        ColumnLayout {
            Label {
                text: "No. Stacks: " + stackSlider.value.toFixed(0)
            }
            Slider {
                id: stackSlider
                focusPolicy: Qt.NoFocus
                from: 1
                to: 9
                value: 4
                stepSize: 1
                snapMode: Slider.SnapOnRelease
            }
            Button {
                id: resetButton
                Layout.alignment: Qt.AlignHCenter
                text: "Reset scene"
                onClicked: shapeSpawner.reset()
            }
            Button {
                id: fireButton
                Layout.alignment: Qt.AlignHCenter
                text: "Fire!"
                onClicked: shapeSpawner.createBall(camera.position,
                                                   camera.forward)
            }
        }
    }
}
