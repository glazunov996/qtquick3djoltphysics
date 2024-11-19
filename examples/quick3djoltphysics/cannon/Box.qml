import QtQuick
import QtQuick3D
import QtQuick3D.JoltPhysics

Body {
    property real xyzExtents: 1
    restitution: 0.6
    friction: 0.5
    motionType: Body.Dynamic
    objectLayer: moving
    motionQuality: Body.LinearCast

    Model {
        source: "#Cube"
        scale: Qt.vector3d(xyzExtents, xyzExtents, xyzExtents).times(0.01)
        materials: PrincipledMaterial {
            baseColor: "red"
        }
    }

    shape: BoxShape {
        extents: Qt.vector3d(xyzExtents, xyzExtents, xyzExtents)
        density: 10000
    }
}
