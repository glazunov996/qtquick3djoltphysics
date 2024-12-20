import QtQuick
import QtQuick3D
import QtQuick3D.JoltPhysics

Item {
    id: root

    property Node origin
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

    property bool moveForward: false
    property bool moveBack: false
    property bool moveLeft: false
    property bool moveRight: false
    property bool shiftDown: false

    property alias acceptedButtons: tapHandler.acceptedButtons

    property bool crouchActive: false

    TapHandler {
        id: tapHandler
        onTapped: mousePressed();
    }

    Keys.onPressed: (event)=> { if (!event.isAutoRepeat) handleKeyPress(event) }
    Keys.onReleased: (event)=> { if (!event.isAutoRepeat) handleKeyRelease(event) }

    function mousePressed() {
        root.forceActiveFocus()
        status.useMouse = true
        AppSettings.showSettingsView = false;
    }

    function forwardPressed() {
        moveForward = true
        moveBack = false
    }

    function forwardReleased() {
        moveForward = false
    }

    function backPressed() {
        moveBack = true
        moveForward = false
    }

    function backReleased() {
        moveBack = false
    }

    function rightPressed() {
        moveRight = true
        moveLeft = false
    }

    function rightReleased() {
        moveRight = false
    }

    function leftPressed() {
        moveLeft = true
        moveRight = false
    }

    function leftReleased() {
        moveLeft = false
    }

    function shiftPressed() {
        shiftDown = true
    }

    function shiftReleased() {
        shiftDown = false
    }

    function sneakPressed() {
        crouchActive = true
    }

    function sneakReleased() {
        crouchActive = false
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
            sneakPressed();
            break
        case Qt.Key_Escape:
            escapePressed();
            break;
        case Qt.Key_P:
            AppSettings.playPhysics = !AppSettings.playPhysics;
            break;
        case Qt.Key_R:
            status.useMouse = false;
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
            sneakReleased();
            break;
        }
    }

    FrameAnimation {
        id: updateTimer
        running: true
        onTriggered: status.processInput(frameTime * 100)
    }

    QtObject {
        id: status

        property bool useMouse: false

        onUseMouseChanged: {
            if (useMouse)
                _mouseHandler.grabMouse(rootWindow);
            else
                _mouseHandler.releaseMouse(rootWindow);
        }

        function processInput(frameDelta) {
            if (useMouse) {
                // Get the delta
                var rotationVector = root.origin.eulerRotation;
                var delta = _mouseHandler.getMouseDelta();
                // rotate x
                var rotateX = delta.x * xSpeed * frameDelta
                if (xInvert)
                    rotateX = -rotateX;
                rotationVector.y += rotateX;

                // rotate y
                var rotateY = delta.y * -ySpeed * frameDelta
                if (yInvert)
                    rotateY = -rotateY;
                rotationVector.z += rotateY;
                origin.setEulerRotation(rotationVector);
            }
        }
    }
}
