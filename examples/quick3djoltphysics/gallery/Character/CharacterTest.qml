import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
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

    property var bodies: []
    property var rampBlocks: []

    readonly property real worldScale: 0.2

    readonly property real characterHeightStanding: 1.35
    readonly property real characterRadiusStanding: 0.3

    property bool controlMovementDuringJump: true
    property real characterSpeed: 6.0
    property real jumpSpeed: 4.0

    readonly property real collisionTolerance: 0.05

    property real rampBlocksTimeLeft: 0

    function radiansToDegrees(radians) {
        return radians * (180 / Math.PI);
    }

    function multiply3x3(mat, vec) {
        var resultX = mat.m11 * vec.x + mat.m12 * vec.y + mat.m13 * vec.z;
        var resultY = mat.m21 * vec.x + mat.m22 * vec.y + mat.m23 * vec.z;
        var resultZ = mat.m31 * vec.x + mat.m32 * vec.y + mat.m33 * vec.z;
        return Qt.vector3d(resultX, resultY, resultZ);
    }

    function initCameraPosition(forward) {
        var normalizedForward = forward.normalized()

        var yaw = Math.atan2(normalizedForward.z, normalizedForward.x);
        var pitch = Math.atan2(normalizedForward.y, Qt.vector3d(normalizedForward.x, 0, normalizedForward.z).length());

        var fwd = Qt.vector3d(Math.cos(pitch) * Math.cos(yaw), Math.sin(pitch), Math.cos(pitch) * Math.sin(yaw));
        var pivot = Qt.matrix4x4();
        pivot.translate(Qt.vector3d(0, characterHeightStanding + characterRadiusStanding, 0).minus(fwd.times(5)));
        var pos = pivot.times(Qt.vector3d(0, 0, 0));

        normalizedForward = multiply3x3(pivot, normalizedForward);
        yaw = Math.atan2(normalizedForward.z, normalizedForward.x);
        pitch = Math.atan2(normalizedForward.y, Qt.vector3d(normalizedForward.x, 0, normalizedForward.z).length());

        var roll = camera.eulerRotation.z;
        var rotation = Qt.vector3d(radiansToDegrees(pitch), radiansToDegrees(yaw) - 90, roll);
        camera.setEulerRotation(rotation);
        camera.position = pos;
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
        onFrameDone: (deltaTime) => postPhysicsUpdate(deltaTime)
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

    function isSupported() {
        return character.groundState === CharacterVirtual.OnGround || character.groundState === CharacterVirtual.OnSteepGround;
    }

    function handleInput(frameDelta) {
        var speed = (characterController.shiftDown ? characterController.shiftSpeed : characterController.speed);
        var forwardSpeed = (characterController.moveForward ? 1 : characterController.moveBack ? -1 : 0);
        var sideSpeed = (characterController.moveLeft ? 1 : characterController.moveRight ? -1 : 0);
        var movementDirection = Qt.vector3d(sideSpeed, 0, forwardSpeed);
        var rotation = Qt.matrix4x4();
        rotation.rotate(originNode.eulerRotation.y + 90, Qt.vector3d(0, 1, 0));
        movementDirection = rotation.times(movementDirection);

        var groundState = character.getGroundState();
        if (groundState === Character.OnSteepGround || groundState === Character.NotSupported) {
            var groundNormal = character.getGroundNormal();
            var normal = Qt.vector3d(groundNormal.x, groundNormal.y, groundNormal.z);
            normal.y = 0;
            var dot = normal.dotProduct(movementDirection);
            if (dot < 0)
                movementDirection = movementDirection.minus(normal.times(dot)).times(1 / normal.length());
        }

        if (controlMovementDuringJump || isSupported()) {
            var currentVelocity = character.getLinearVelocity();
            var desiredVelocity = Qt.vector3d(movementDirection.x, movementDirection.y, movementDirection.z).times(speed);
            desiredVelocity.y = currentVelocity.y;
            var newVelocity = currentVelocity.times(0.75).plus(desiredVelocity.times(0.25))
            character.setLinearVelocity(newVelocity);
        }
    }

    function prePhysicsUpdate(frameDelta) {
        rampBlocksTimeLeft -= frameDelta / 1000;
        if (rampBlocksTimeLeft < 0.0) {
            for (var i = 0; i < 4; ++i) {
                var rampBlock = rampBlocks[i];
                var position = Qt.vector3d(15, 2.2, 15).plus(Qt.vector3d(-3.0, 3, 1.5))
                rampBlock.position = position.plus(Qt.vector3d(2, 0, 0).times(i));
                rampBlock.eulerRotation = Qt.vector3d(-45, 0, 0);
                rampBlock.setLinearVelocity(Qt.vector3d(0, 0, 0));
            }

            rampBlocksTimeLeft = 5.0;
        }

        handleInput(frameDelta);

        var groundState = character.getGroundState();
        if (groundState === Character.OnGround)
            stateText.text = "State: OnGround"
        else if (groundState === Character.InAir)
            stateText.text = "State: InAir"
        else if (groundState === Character.OnSteepGround)
            stateText.text = "State: OnSteepGround"
        else
            stateText.text = "State: NotSupported"

        var characterVelocity = character.getLinearVelocity();
        var horizontalVelocity = Qt.vector3d(characterVelocity.x, 0, characterVelocity.z);

        horizontalVelText.text = "Horizontal Vel: " + horizontalVelocity.length().toFixed(2) + " m/s";
        verticalVelText.text = "Vertical Vel: " + characterVelocity.y.toFixed(2) + " m/s"
    }

    function postPhysicsUpdate(frameDelta) {
        character.postSimulation(collisionTolerance);
    }

    SampleCharacterCameraController {
        id: characterController
        origin: originNode
        camera: camera
        speed: characterSpeed
        shiftSpeed: characterSpeed * 3
    }

    View3D {
        id: viewport
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#191970"
            backgroundMode: SceneEnvironment.Color
        }

        Node {
            id: originNode
            PerspectiveCamera {
                id: camera
                clipFar: 50000
                clipNear: 1
                fieldOfView: 70

                Component.onCompleted:  {
                    initCameraPosition(Qt.vector3d(10, -2, 0))
                }
            }
            position: character.position
        }

        Character {
            id: character
            shape: CapsuleShape {
                height: characterHeightStanding
                diameter: characterRadiusStanding * 2
            }
            layer: moving
            friction: 0.5
            maxSlopeAngle: 45
            supportingVolume: Qt.vector4d(0, 1, 0, -characterRadiusStanding)
            Model {
                eulerRotation.z: -90
                geometry: CapsuleGeometry {
                    height: characterHeightStanding
                    diameter: characterRadiusStanding * 2
                }
                materials: PrincipledMaterial {
                    metalness: 0.5
                    roughness: 0.1
                    baseColor: "blue"
                }
            }
            Node {
                eulerRotation.y: originNode.eulerRotation.y - 90
                Node {
                    position.y: 1.0
                    position.x: 1.65
                    Item {
                        width: 260
                        height: 120
                        anchors.centerIn: parent
                        Rectangle {
                            anchors.fill: parent
                            opacity: 0.4
                            color: "#202020"
                            radius: 10
                            border.width: 1
                            border.color: "#f0f0f0"
                        }
                        Column {
                            id: content
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.topMargin: 10
                            anchors.leftMargin: 10
                            spacing: 2
                            Text {
                                id: stateText
                                text: "State: InAir"
                                color: "#e0e0e0"
                                style: Text.Raised
                                font.pixelSize: 20
                            }
                            Text {
                                text: "Mat: Default"
                                color: "#e0e0e0"
                                style: Text.Raised
                                font.pixelSize: 20
                            }
                            Text {
                                id: horizontalVelText
                                text: "Horizontal Vel: 0.0 m/s"
                                color: "#e0e0e0"
                                style: Text.Raised
                                font.pixelSize: 20
                            }
                            Text {
                                id: verticalVelText
                                text: "Vertical Vel: 0.0 m/s"
                                color: "#e0e0e0"
                                style: Text.Raised
                                font.pixelSize: 20
                            }
                        }
                    }
                    scale: Qt.vector3d(0.01, 0.01, 0.01)
                }
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
                extents: Qt.vector3d(350 * worldScale, 2, 350 * worldScale)
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
                scale: Qt.vector3d(3.5 * worldScale, 0.02, 3.5 * worldScale)
            }
        }

        Body {
            id: body1
            shape: SphereShape {
                diameter: 0.4
            }
            objectLayer: moving
            motionType: Body.Dynamic
            position: Qt.vector3d(0, 0.2, -1.0)
            mass: 10.0
            overrideMassProperties: Body.CalculateInertia
            Model {
                source: "#Sphere"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[body1.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.004, 0.004, 0.004)
            }
            onBodyIDChanged: {
                body1.setLinearVelocity(Qt.vector3d(0, 0, 2.0))
            }
        }

        Component {
            id: rampBodyComponent
            Node {
                property alias bodyPosition: rampBody.position
                property alias bodyExtents: rampBodyShape.extents
                Body {
                    id: rampBody
                    shape: BoxShape {
                        id: rampBodyShape
                    }
                    objectLayer: nonMoving
                    motionType: Body.Static
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
                        scale: Qt.vector3d(bodyExtents.x / 100, bodyExtents.y / 100, bodyExtents.z / 100)
                    }
                }
            }
        }

        Component {
            id: rampBlockComponent
            Body {
                id: body
                shape: BoxShape {
                    extents: Qt.vector3d(1, 1, 1)
                }
                objectLayer: moving
                motionType: Body.Dynamic
                mass: 10
                overrideMassProperties: Body.CalculateInertia
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
                    scale: Qt.vector3d(0.01, 0.01, 0.01)
                }
            }
        }
        ExampleStairsGridGeometry {
            id: stairsGridGeometry
        }

        Body {
            shape: MeshShape {
                geometry: stairsGridGeometry
            }
            objectLayer: nonMoving
            motionType: Body.Static
            position: Qt.vector3d(-20.0, 0, 2.5)
            Model {
                geometry: stairsGridGeometry
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
            }
        }

        Component.onCompleted: {
            // Create ramps with different inclinations
            for (var angle = 0; angle < 18; ++angle) {
                var position = Qt.vector3d(-15.0 + angle * 2, 0, -10.0)
                var rotation = Qt.vector3d(10 * angle, 0, 0)
                var ramp = rampBodyComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation, bodyPosition: Qt.vector3d(0, 0, -2.5), bodyExtents: Qt.vector3d(2, 0.1, 5) })
                bodies.push(ramp)
            }
            // Create ramps with different inclinations intersecting with a steep slope
            for (angle = 0; angle < 9; ++angle) {
                position = Qt.vector3d(-15.0 + angle * 2, 0, -20.0 - angle * 0.1)
                rotation = Qt.vector3d(10 * angle, 0, 0)
                ramp = rampBodyComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation, bodyPosition: Qt.vector3d(0, 0, -2.5), bodyExtents: Qt.vector3d(2, 0.1, 5) })
                bodies.push(ramp)
                position = Qt.vector3d(-15.0 + angle * 2, 0, -21)
                rotation = Qt.vector3d(0, 0, 20)
                ramp = rampBodyComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation, bodyPosition: Qt.vector3d(0, 2, 0), bodyExtents: Qt.vector3d(0.1, 4, 2) })
                bodies.push(ramp)
            }

            // Create ramp
            position = Qt.vector3d(15, 2.2, 15)
            rotation = Qt.vector3d(-45, 0, 0)
            ramp = rampBodyComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation, bodyPosition: Qt.vector3d(0, 0, 0), bodyExtents: Qt.vector3d(8, 0.2, 6) })
            bodies.push(ramp)

            // Create blocks on ramp
            position = position.plus(Qt.vector3d(-3.0, 3, 1.5))
            rotation = Qt.vector3d(-45, 0, 0)
            for (var i = 0; i < 4; ++i) {
                var rampBlock = rampBlockComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation })
                position = position.plus(Qt.vector3d(2.0, 0, 0))
                bodies.push(rampBlock)
                rampBlocks.push(rampBlock)
            }
        }
    }

    SettingsView {}
}
