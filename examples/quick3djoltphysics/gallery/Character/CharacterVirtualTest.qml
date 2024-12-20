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

    property real time: 0

    readonly property int nonMoving : 0
    readonly property int moving: 1
    readonly property int debris: 2
    readonly property int sensor: 3

    property var bodies: []
    property var rampBlocks: []
    property alias rampBlockIDs: characterContactListener.rampBlockIDs

    readonly property real worldScale: 0.2

    readonly property real characterHeightStanding: 1.35
    readonly property real characterRadiusStanding: 0.3

    readonly property real characterHeightCrouching: 0.8
    readonly property real characterRadiusCrouching: 0.3

    property bool enableCharacterInertia: true
    property real upRotationX: 0
    property real upRotationZ: 0

    property bool controlMovementDuringJump: true
    property real characterSpeed: 6.0
    property real jumpSpeed: 4.0

    property bool enableWalkStairs: true
    property bool enableStickToFloor: true

    property vector3d desiredVelocity: Qt.vector3d(0, 0, 0)
    property alias allowSliding: characterContactListener.allowSliding

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
        gravity: Qt.vector3d(0, -9.81, 0)
        collisionSteps: 1
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
        onBeforeFrameDone: (deltaTime) => prePhysicsUpdate(deltaTime)
    }

    Component {
        id: extendedUpdateSettingsComponent
        ExtendedUpdateSettings {}
    }

    function isSupported() {
        var groundState = character.getGroundState();
        return groundState === CharacterVirtual.OnGround || groundState === CharacterVirtual.OnSteepGround;
    }

    function handleInput(frameDelta) {
        var speed = (characterController.shiftDown ? characterController.shiftSpeed : characterController.speed);
        var forwardSpeed = (characterController.moveForward ? 1 : characterController.moveBack ? -1 : 0);
        var sideSpeed = (characterController.moveLeft ? 1 : characterController.moveRight ? -1 : 0);
        var movementDirection = Qt.vector3d(sideSpeed, 0, forwardSpeed);
        var rotation = Qt.matrix4x4();
        rotation.rotate(originNode.eulerRotation.y + 90, Qt.vector3d(0, 1, 0));
        movementDirection = rotation.times(movementDirection);

        var playerControlsHorizontalVelocity = controlMovementDuringJump || isSupported()

        if (playerControlsHorizontalVelocity) {
            desiredVelocity = Qt.vector3d(movementDirection.x, movementDirection.y, movementDirection.z).times(speed)
            allowSliding = !desiredVelocity.fuzzyEquals(Qt.vector3d(0, 0, 0), 1.0e-12);
        } else {
            allowSliding = true;
        }

        var characterUpRotation = Quaternion.fromEulerAngles(upRotationX, 0, upRotationZ);
        var up = characterUpRotation.times(Qt.vector3d(0, 1, 0));
        character.setRotation(characterUpRotation);
        character.updateGroundVelocity();

        var currentVelocity = character.getLinearVelocity();
        var currentVerticalVelocity = character.up.times(currentVelocity.dotProduct(character.up));
        var groundVelocity = character.getGroundVelocity();
        var newVelocity;
        var movingTowardsGround = (currentVerticalVelocity.y - groundVelocity.y) < 0.1;
        if (character.getGroundState() === CharacterVirtual.OnGround
                && (enableCharacterInertia ?
                        movingTowardsGround :
                        !character.isSlopeTooSteep(character.getGroundNormal()))) {
            newVelocity = groundVelocity;
        } else {
            newVelocity = currentVerticalVelocity;
        }

        newVelocity = newVelocity.plus(physicsSystem.gravity.times(frameDelta));

        if (playerControlsHorizontalVelocity) {
            newVelocity = newVelocity.plus(characterUpRotation.times(desiredVelocity));
        } else {
            var currentHorizontalVelocity = currentVelocity.minus(currentVerticalVelocity);
            newVelocity = newVelocity.plus(currentHorizontalVelocity);
        }

        character.setLinearVelocity(newVelocity);
    }

    function prePhysicsUpdate(frameDelta) {
        time += frameDelta;

        smoothVerticallyMovingBody.kinematicPosition = Qt.vector3d(0, 2.0, 15).plus(Qt.vector3d(0, 1.75 * Math.sin(time), 0));

        rampBlocksTimeLeft -= frameDelta;
        if (rampBlocksTimeLeft < 0.0) {
            for (var i = 0; i < 4; ++i) {
                var rampBlock = rampBlocks[i];
                var position = Qt.vector3d(15, 2.2, 15).plus(Qt.vector3d(-3.0, 3, 1.5))
                rampBlock.reset(position.plus(Qt.vector3d(2, 0, 0).times(i)), Qt.vector3d(-45, 0, 0))
                rampBlock.setLinearVelocity(Qt.vector3d(0, 0, 0))
            }

            rampBlocksTimeLeft = 5.0;
        }

        handleInput(frameDelta);

        var updateSettings = extendedUpdateSettingsComponent.createObject(null);

        if (!enableStickToFloor)
            updateSettings.stickToFloorStepDown = Qt.vector3d(0, 0, 0);
        else
            updateSettings.stickToFloorStepDown = character.up.times(-1).times(updateSettings.stickToFloorStepDown.length())

        if (!enableWalkStairs)
            updateSettings.walkStairsStepUp = Qt.vector3d(0, 0, 0);
        else
            updateSettings.walkStairsStepUp = character.up.times(updateSettings.walkStairsStepUp.length())

        character.extendedUpdate(frameDelta,
                                 Qt.vector3d(0, -1, 0).times(physicsSystem.gravity.length()),
                                 updateSettings,
                                 moving,
                                 moving);

        updateSettings.destroy();

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

    SampleCharacterCameraController {
        id: characterController
        origin: originNode
        camera: camera
        speed: characterSpeed
        shiftSpeed: characterSpeed * 3
        onCrouchActiveChanged: {
            if (characterController.crouchActive) {
                character.shape = crouchingCapsuleShape;
                character.innerBodyShape = crouchingInnerBodyCapsuleShape;
            } else {
                character.shape = capsuleShape;
                character.innerBodyShape = innerBodyCapsuleShape;
            }
            character.setShape(1.5 * 0.02, moving, moving);
        }
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

        RotatedTranslatedShape {
            id: capsuleShape
            shape: CapsuleShape {
                height: characterHeightStanding
                diameter: characterRadiusStanding * 2
            }
            position: Qt.vector3d(0, 0.5 * characterHeightStanding + characterRadiusStanding, 0)
        }

        RotatedTranslatedShape {
            id: innerBodyCapsuleShape
            shape: CapsuleShape {
                height: characterHeightStanding
                diameter: characterRadiusStanding * 2
            }
            position: Qt.vector3d(0, 0.5 * characterHeightStanding + characterRadiusStanding, 0)
        }

        RotatedTranslatedShape {
            id: crouchingCapsuleShape
            shape: CapsuleShape {
                height: characterHeightCrouching
                diameter: characterRadiusCrouching * 2
            }
            position: Qt.vector3d(0, 0.5 * characterHeightCrouching + characterRadiusCrouching, 0)
        }

        RotatedTranslatedShape {
            id: crouchingInnerBodyCapsuleShape
            shape: CapsuleShape {
                height: characterHeightCrouching
                diameter: characterRadiusCrouching * 2
            }
            position: Qt.vector3d(0, 0.5 * characterHeightCrouching + characterRadiusCrouching, 0)
        }

        CharacterVirtual {
            id: character
            shape: capsuleShape
            innerBodyShape: innerBodyCapsuleShape
            characterContactListener: ExampleCharacterContactListener {
                id: characterContactListener
                onEnableSliding: (canPushCharacter, bodyID2) => {
                    for (var i = 0; i < bodies.length; ++i) {
                        var body = bodies[i];
                        if (body.bodyID === bodyID2 && body.motionType !== Body.Static)
                            allowSliding = true;
                    }
                }
            }
            maxSlopeAngle: 45
            maxStrength: 100
            backFaceMode: JoltPhysics.CollideWithBackFaces
            characterPadding: 0.02
            penetrationRecoverySpeed: 1.0
            predictiveContactDistance: 0.1
            supportingVolume: Qt.vector4d(0, 1, 0, -characterRadiusStanding)
            mass: 10
            Model {
                eulerRotation.z: -90
                geometry: CapsuleGeometry {
                    height: !characterController.crouchActive ? characterHeightStanding : characterHeightCrouching
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
            id: rollingSphere
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
                    baseColor: colors[rollingSphere.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.004, 0.004, 0.004)
            }
            onBodyIDChanged: {
                rollingSphere.setLinearVelocity(Qt.vector3d(0, 0, 2.0))
            }
        }

        Repeater3D {
            model: 3
            Body {
                id: dynamicBlock
                shape: BoxShape {
                    extents: Qt.vector3d(1, 1, 1)
                }
                objectLayer: moving
                motionType: Body.Dynamic
                position: Qt.vector3d(5.0, 0.5 + index, 0.0)
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
                        baseColor: colors[dynamicBlock.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                    scale: Qt.vector3d(0.01, 0.01, 0.01)
                }
            }
        }

        Component {
            id: rampComponent
            Node {
                property alias bodyPosition: ramp.position
                property alias bodyExtents: rampShape.extents
                Body {
                    id: ramp
                    shape: BoxShape {
                        id: rampShape
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
                id: rampBlock
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
                        baseColor: colors[rampBlock.bodyID % colors.length]
                        metalness: 0.5
                        roughness: 0.1
                    }
                    scale: Qt.vector3d(0.01, 0.01, 0.01)
                }
            }
        }

        Component {
            id: floatingStaticBlockComponent
            Body {
                id: floatingStaticBlock
                shape: BoxShape {
                    extents: Qt.vector3d(1, 1, 1)
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
                        baseColor: colors[floatingStaticBlock.bodyID % colors.length]
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

        Body {
            id: smoothVerticallyMovingBody
            shape: BoxShape {
                extents: Qt.vector3d(1, 0.15, 3.0)
            }
            objectLayer: moving
            motionType: Body.Kinematic
            position: Qt.vector3d(0, 2.0, 15)
            Model {
                source: "#Cube"
                materials: PrincipledMaterial {
                    baseColorMap: Texture {
                        source: "qrc:/images/checkers2.png"
                        scaleU: 2
                        scaleV: 2
                    }
                    baseColor: colors[smoothVerticallyMovingBody.bodyID % colors.length]
                    metalness: 0.5
                    roughness: 0.1
                }
                scale: Qt.vector3d(0.01, 0.0015, 0.03)
            }
        }

        Component.onCompleted: {
            // Create ramps with different inclinations
            for (var angle = 0; angle < 18; ++angle) {
                var position = Qt.vector3d(-15.0 + angle * 2, 0, -10.0)
                var rotation = Qt.vector3d(10 * angle, 0, 0)
                var ramp = rampComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation, bodyPosition: Qt.vector3d(0, 0, -2.5), bodyExtents: Qt.vector3d(2, 0.1, 5) })
                bodies.push(ramp)
            }
            // Create ramps with different inclinations intersecting with a steep slope
            for (angle = 0; angle < 9; ++angle) {
                position = Qt.vector3d(-15.0 + angle * 2, 0, -20.0 - angle * 0.1)
                rotation = Qt.vector3d(10 * angle, 0, 0)
                ramp = rampComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation, bodyPosition: Qt.vector3d(0, 0, -2.5), bodyExtents: Qt.vector3d(2, 0.1, 5) })
                bodies.push(ramp)
                position = Qt.vector3d(-15.0 + angle * 2, 0, -21)
                rotation = Qt.vector3d(0, 0, 20)
                ramp = rampComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation, bodyPosition: Qt.vector3d(0, 2, 0), bodyExtents: Qt.vector3d(0.1, 4, 2) })
                bodies.push(ramp)
            }
            // A floating static block
            var body = floatingStaticBlockComponent.createObject(physicsSystem.scene, { position: Qt.vector3d(30.0, 1.5, 0.0) })

            // Create ramp
            position = Qt.vector3d(15, 2.2, 15)
            rotation = Qt.vector3d(-45, 0, 0)
            ramp = rampComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation, bodyPosition: Qt.vector3d(0, 0, 0), bodyExtents: Qt.vector3d(8, 0.2, 6) })
            bodies.push(ramp)

            // Create blocks on ramp
            position = position.plus(Qt.vector3d(-3.0, 3, 1.5))
            rotation = Qt.vector3d(-45, 0, 0)
            for (var i = 0; i < 4; ++i) {
                var rampBlock = rampBlockComponent.createObject(physicsSystem.scene, { position: position, eulerRotation: rotation })
                position = position.plus(Qt.vector3d(2.0, 0, 0))
                bodies.push(rampBlock)
                rampBlocks.push(rampBlock)
                rampBlockIDs.push(rampBlock.bodyID)
            }
        }
    }

    SettingsView {}
}
