pragma ComponentBehavior: Bound

import QtQuick
import QtQuick3D
import QtQuick3D.Particles3D
import QtQuick.Controls
import QtQuick3D.JoltPhysics
import Example

Item {
    id: mainView

    required property Loader loader

    readonly property real listItemWidth: 180
    readonly property real listItemHeight: 40

    anchors.fill: parent

    ListModel {
        id: generalModel
        ListElement {
            name: "Simple"
            file: "General/SimpleTest.qml"
        }
        ListElement {
            name: "Stack"
            file: "General/StackTest.qml"
        }
        ListElement {
            name: "Wall"
            file: "General/WallTest.qml"
        }
        ListElement {
            name: "Pyramid"
            file: "General/PyramidTest.qml"
        }
        ListElement {
            name: "2D Funnel"
            file: "General/TwoDFunnelTest.qml"
        }
        ListElement {
            name: "Friction"
            file: "General/FrictionTest.qml"
        }
        ListElement {
            name: "Gravity Factor"
            file: "General/GravityFactorTest.qml"
        }
        ListElement {
            name: "Restitution"
            file: "General/RestitutionTest.qml"
        }
        ListElement {
            name: "Damping"
            file: "General/DampingTest.qml"
        }
        ListElement {
            name: "High Speed"
            file: "General/HighSpeedTest.qml"
        }
        ListElement {
            name: "Kinematic"
            file: "General/KinematicTest.qml"
        }
        ListElement {
            name: "Contact Manifold"
            file: "General/ContactManifoldTest.qml"
        }
        ListElement {
            name: "Change Motion Quality"
            file: "General/ChangeMotionQualityTest.qml"
        }
        ListElement {
            name: "Change Object Layer"
            file: "General/ChangeObjectLayerTest.qml"
        }
        ListElement {
            name: "Change Shape"
            file: "General/ChangeShapeTest.qml"
        }
        ListElement {
            name: "Contact Listener"
            file: "General/ContactListenerTest.qml"
        }
        ListElement {
            name: "Sensor"
            file: "General/SensorTest.qml"
        }
        ListElement {
            name: "Center Of Mass"
            file: "General/CenterOfMassTest.qml"
        }
    }

    ListModel {
        id: shapesModel
        ListElement {
            name: "Shpere Shape"
            file: "Shapes/SphereShapeTest.qml"
        }
        ListElement {
            name: "Box Shape"
            file: "Shapes/BoxShapeTest.qml"
        }
        ListElement {
            name: "Capsule Shape"
            file: "Shapes/CapsuleShapeTest.qml"
        }
        ListElement {
            name: "Cylinder Shape"
            file: "Shapes/CylinderShapeTest.qml"
        }
        ListElement {
            name: "Convex Hull Shape"
            file: "Shapes/ConvexHullShapeTest.qml"
        }
        ListElement {
            name: "Mesh Shape"
            file: "Shapes/MeshShapeTest.qml"
        }
        ListElement {
            name: "Static Compound Shape"
            file: "Shapes/StaticCompoundShapeTest.qml"
        }
        ListElement {
            name: "Offset Center Of Mass Shape"
            file: "Shapes/OffsetCenterOfMassShapeTest.qml"
        }
        ListElement {
            name: "Plane Shape"
            file: "Shapes/PlaneShapeTest.qml"
        }
    }

    ListModel {
        id: scaledShapesModel
        ListElement {
            name: "Scaled Sphere Shape"
            file: "ScaledShapes/ScaledSphereShapeTest.qml"
        }
        ListElement {
            name: "Scaled Box Shape"
            file: "ScaledShapes/ScaledBoxShapeTest.qml"
        }
        ListElement {
            name: "Scaled Capsule Shape"
            file: "ScaledShapes/ScaledCapsuleShapeTest.qml"
        }
        ListElement {
            name: "Scaled Cylinder Shape"
            file: "ScaledShapes/ScaledCylinderShapeTest.qml"
        }
    }

    ListModel {
        id: constraintsModel
        ListElement {
            name: "Point Constraint"
            file: "Constraints/PointConstraintTest.qml"
        }
        ListElement {
            name: "Distance Constraint"
            file: "Constraints/DistanceConstraintTest.qml"
        }
        ListElement {
            name: "Hinge Constraint"
            file: "Constraints/HingeConstraintTest.qml"
        }
        ListElement {
            name: "Swing Twist Constraint"
            file: "Constraints/SwingTwistConstraintTest.qml"
        }
        ListElement {
            name: "Fixed Constraint"
            file: "Constraints/FixedConstraintTest.qml"
        }
    }

    ListModel {
        id: characterModel
        ListElement {
            name: "Character"
            file: "Character/CharacterTest.qml"
        }
        ListElement {
            name: "Character Virtual"
            file: "Character/CharacterVirtualTest.qml"
        }
    }

    ListModel {
        id: quick3dphyiscsModel
        ListElement {
            name: "Simple"
            file: "Quick3DPhysics/Simple.qml"
        }
    }

    Component {
        id: listComponent
        Button {
            id: button
            required property string name
            required property string file

            width: mainView.listItemWidth
            height: mainView.listItemHeight
            background: Rectangle {
                id: buttonBackground
                border.width: 0.5
                border.color: "#d0808080"
                color: "#d0404040"
                opacity: button.hovered ? 1.0 : 0.5
            }
            contentItem: Text {
                anchors.centerIn: parent
                color: "#f0f0f0"
                font.pointSize: AppSettings.fontSizeSmall
                text: button.name
            }

            onClicked: {
                mainView.loader.source = button.file
            }
        }
    }

    Text {
        id: topLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        text: qsTr("Qt Quick 3D Jolt Physics - Gallery")
        color: "#f0f0f0"
        font.pointSize: AppSettings.fontSizeLarge
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topLabel.bottom
        anchors.topMargin: 20
        spacing: 20
        ListView {
            width: mainView.listItemWidth
            height: count * mainView.listItemHeight
            model: generalModel
            delegate: listComponent
        }

        ListView {
            width: mainView.listItemWidth
            height: count * mainView.listItemHeight
            model: shapesModel
            delegate: listComponent
        }

        ListView {
            width: mainView.listItemWidth
            height: count * mainView.listItemHeight
            model: scaledShapesModel
            delegate: listComponent
        }

        ListView {
            width: mainView.listItemWidth
            height: count * mainView.listItemHeight
            model: constraintsModel
            delegate: listComponent
        }

        ListView {
            width: mainView.listItemWidth
            height: count * mainView.listItemHeight
            model: characterModel
            delegate: listComponent
        }

        ListView {
            width: mainView.listItemWidth
            height: count * mainView.listItemHeight
            model: quick3dphyiscsModel
            delegate: listComponent
        }
    }
}
