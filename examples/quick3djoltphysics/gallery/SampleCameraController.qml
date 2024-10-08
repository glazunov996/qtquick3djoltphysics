import QtQuick
import QtQuick3D
import QtQuick3D.JoltPhysics

Item {
    id: root

    property Camera camera
    property PhysicsSystem physicsSystem

    property real speed: 1
    property real shiftSpeed: 3

    property real forwardSpeed: 3
    property real backSpeed: 3
    property real rightSpeed: 3
    property real leftSpeed: 3
    property real xSpeed: 0.1
    property real ySpeed: 0.1

    property bool xInvert: false
    property bool yInvert: true

    property real dragRayLength: 40.0

    implicitWidth: parent.width
    implicitHeight: parent.height
    focus: true

    TapHandler {
        id: tapHandler
        onTapped: mousePressed();
    }

    Keys.onPressed: (event)=> { if (!event.isAutoRepeat) handleKeyPress(event) }
    Keys.onReleased: (event)=> { if (!event.isAutoRepeat) handleKeyRelease(event) }

    function mousePressed() {
        root.forceActiveFocus()
        status.currentPos = _cursor.pos()
        status.lastPos = _cursor.pos()
        status.useMouse = true
        AppSettings.showSettingsView = false;
    }

    function forwardPressed() {
        status.moveForward = true
        status.moveBack = false
    }

    function forwardReleased() {
        status.moveForward = false
    }

    function backPressed() {
        status.moveBack = true
        status.moveForward = false
    }

    function backReleased() {
        status.moveBack = false
    }

    function rightPressed() {
        status.moveRight = true
        status.moveLeft = false
    }

    function rightReleased() {
        status.moveRight = false
    }

    function leftPressed() {
        status.moveLeft = true
        status.moveRight = false
    }

    function leftReleased() {
        status.moveLeft = false
    }

    function shiftPressed() {
        status.shiftDown = true
    }

    function shiftReleased() {
        status.shiftDown = false
    }

    function controlPressed() {
        status.controlDown = true
    }

    function controlReleased() {
        status.controlDown = false
    }

    function escapePressed() {
        status.useMouse = false
        AppSettings.showSettingsView = true;
    }

    function handleKeyPress(event)
    {
        switch (event.key) {
        case Qt.Key_W:
        case Qt.Key_Up:
            forwardPressed();
            break;
        case Qt.Key_S:
        case Qt.Key_Down:
            backPressed();
            break;
        case Qt.Key_A:
        case Qt.Key_Left:
            leftPressed();
            break;
        case Qt.Key_D:
        case Qt.Key_Right:
            rightPressed();
            break;
        case Qt.Key_Shift:
            shiftPressed();
            break;
        case Qt.Key_Control:
            controlPressed();
            break
        case Qt.Key_Escape:
            escapePressed();
            break;
        case Qt.Key_P:
            AppSettings.playPhysics = !AppSettings.playPhysics;
            break;
        case Qt.Key_R:
            AppSettings.testRestartNeeded()
            break;
        }
    }

    function handleKeyRelease(event)
    {
        switch (event.key) {
        case Qt.Key_W:
        case Qt.Key_Up:
            forwardReleased();
            break;
        case Qt.Key_S:
        case Qt.Key_Down:
            backReleased();
            break;
        case Qt.Key_A:
        case Qt.Key_Left:
            leftReleased();
            break;
        case Qt.Key_D:
        case Qt.Key_Right:
            rightReleased();
            break;
        case Qt.Key_Shift:
            shiftReleased();
            break;
        case Qt.Key_Control:
            controlReleased();
            break;
        }
    }

    FrameAnimation {
        id: updateTimer
        running: true
        onTriggered: status.processInput(frameTime * 100)
    }

    Component {
        id: markerComponent
        Model {
            source: "#Sphere"
            materials: DefaultMaterial {
                diffuseColor: "red"
                lighting: Light.None
            }
            scale: Qt.vector3d(0.002, 0.002, 0.002)
            castsShadows: false
            visible: false
        }
    }

    Component {
        id: dragConstraintComponent
        DistanceConstraint {
            limitsSpringSettings: SpringSettings {
                frequency: 2.0
                damping: 1.0
            }
            minDistance: 0
            maxDistance: 0
        }
    }

    Component {
        id: dragAnchorComponent
        Body {
            shape: SphereShape {
                diameter: 0.02
            }
            motionType: Body.Static
            usedInSimulation: false
            objectLayer: 0
        }
    }

    QtObject {
        id: status

        property bool moveForward: false
        property bool moveBack: false
        property bool moveLeft: false
        property bool moveRight: false
        property bool shiftDown: false
        property bool controlDown: false
        property bool useMouse: false

        property point lastPos: Qt.point(0, 0)
        property point currentPos: Qt.point(0, 0)

        property Node marker: null
        property Node dragConstraint: null
        property Node dragAnchor: null

        property var hit: undefined

        function updatePosition(vector, speed, position)
        {
            if (shiftDown)
                speed *= root.shiftSpeed;
            else
                speed *= root.speed

            var direction = vector;
            var velocity = Qt.vector3d(direction.x * speed,
                                       direction.y * speed,
                                       direction.z * speed);
            camera.position = Qt.vector3d(position.x + velocity.x,
                                          position.y + velocity.y,
                                          position.z + velocity.z);
        }

        function negate(vector) {
            return Qt.vector3d(-vector.x, -vector.y, -vector.z)
        }

        function castProbe() {
            var start = camera.position;
            var direction = camera.forward.times(dragRayLength);

            hit = physicsSystem.castRay(start, direction);

            if (hit.body)
                marker.position = hit.position;
            else
                marker.position = camera.position.plus(camera.forward.times(0.1));
        }

        function updateDebug(frameDelta) {
            if (dragConstraint == null) {
                if (marker == null)
                    marker = markerComponent.createObject(physicsSystem.scene);

                castProbe();
                marker.visible = true;

                if (status.controlDown && hit.body) {
                    if (hit.body.motionType === Body.Dynamic) {
                        dragAnchor = dragAnchorComponent.createObject(physicsSystem.scene, { position: hit.position })
                        dragConstraint = dragConstraintComponent.createObject(physicsSystem.scene, {
                                                    point1: hit.position,
                                                    point2: hit.position,
                                                    body1: dragAnchor,
                                                    body2: hit.body})
                    }
                }
            } else {
                if (!status.controlDown) {
                    if (dragConstraint !== null) {
                        dragConstraint.destroy();
                        dragConstraint = null;
                    }
                    if (dragAnchor !== null) {
                        dragAnchor.destroy();
                        dragAnchor = null;
                    }
                    hit = {}
                } else {
                    var newPos = camera.position.plus(camera.forward.times(dragRayLength * hit.fraction))
                    dragAnchor.position = newPos;
                    hit.body.activate();
                }
                marker.visible = false;
            }
        }

        function processInput(frameDelta) {
            if (moveForward)
                updatePosition(root.camera.forward, root.forwardSpeed * frameDelta, root.camera.position);
            else if (moveBack)
                updatePosition(negate(root.camera.forward), root.backSpeed * frameDelta, root.camera.position);

            if (moveRight)
                updatePosition(root.camera.right, root.rightSpeed * frameDelta, root.camera.position);
            else if (moveLeft)
                updatePosition(negate(root.camera.right), root.leftSpeed * frameDelta, root.camera.position);

            if (useMouse) {
                currentPos = _cursor.pos()
                // Get the delta
                var rotationVector = root.camera.eulerRotation;
                var delta = Qt.vector2d(lastPos.x - currentPos.x,
                                        lastPos.y - currentPos.y);
                // rotate x
                var rotateX = delta.x * xSpeed * frameDelta
                if (xInvert)
                    rotateX = -rotateX;
                rotationVector.y += rotateX;

                // rotate y
                var rotateY = delta.y * -ySpeed * frameDelta
                if (yInvert)
                    rotateY = -rotateY;
                rotationVector.x += rotateY;
                camera.setEulerRotation(rotationVector);
                lastPos = currentPos;

                if (status.currentPos.x <= 0 || status.currentPos.x >= root.implicitWidth - 1
                        || status.currentPos.y <= 0 || status.currentPos.y >= root.implicitHeight - 1) {
                    lastPos = Qt.point(root.implicitWidth / 2, root.implicitHeight / 2)
                    _cursor.setPos(lastPos)
                }
            }

            updateDebug(frameDelta)
        }
    }
}
