import QtQuick
import QtQuick3D
import QtQuick3D.JoltPhysics

Body {
    property real sphereDiameter: 2
    restitution: 0.6
    friction: 0.5

    motionType: Body.Dynamic
    objectLayer: moving
    motionQuality: Body.LinearCast

    Model {
        source: "#Sphere"
        scale: Qt.vector3d(1, 1, 1).times(sphereDiameter * 0.01)
        materials: PrincipledMaterial {
            baseColor: "yellow"
        }
    }

    shape: SphereShape {
        diameter: sphereDiameter
        density: 10000
    }
}
