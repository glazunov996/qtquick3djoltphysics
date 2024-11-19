import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick3D.JoltPhysics
import QtQuick3D.JoltPhysics.Helpers as Helpers
import Example

Window {
    width: 1280
    height: 720
    visible: true
    title: qsTr("Qt Quick 3D Jolt Physics - Character Controller")

    readonly property int nonMoving : 0
    readonly property int moving: 1
    readonly property int debris: 2
    readonly property int sensor: 3

    property bool enableCharacterInertia: true
    property bool controlMovementDuringJump: true
    property vector3d desiredVelocity: Qt.vector3d(0, 0, 0)

    PhysicsSystem {
        id: physicsSystem
        scene: viewport.scene
        running: true
        gravity: Qt.vector3d(0, -981, 0)
        objectLayerPairFilter: ExampleObjectLayerPairFilter {}
        broadPhaseLayer: ExampleBroadPhaseLayer {}
        objectVsBroadPhaseLayerFilter: ExampleObjectVsBroadPhaseLayerFilter {}
        contactListener: ExampleSensorContactListener {
            id: contactListener
        }
    }

    View3D {
        id: viewport
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#8090d0"
            backgroundMode: SceneEnvironment.SkyBox
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
            lightProbe: Texture {
                textureData: ProceduralSkyTextureData {}
            }
            specularAAEnabled: true
        }

        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -70
            ambientColor: "#666"
            brightness: 0.1
        }

        Building {
            id: building
            onTeleporterTriggered: character.teleportHome()
        }

        CharacterVirtual {
            id: character
            property vector3d startPos: Qt.vector3d(800, 175 - characterHeight / 2, -850)
            position: startPos
            function teleportHome() {
                character.teleport(character.startPos)
                wasd.cameraRotation.x = 180
            }

            shape: RotatedTranslatedShape {
                shape: CapsuleShape {
                    id: capsuleShape
                    diameter: 50
                    height: 100
                }
                position: Qt.vector3d(0, (capsuleShape.diameter + capsuleShape.height) / 2, 0)
            }
            innerBodyShape: RotatedTranslatedShape {
                shape: CapsuleShape {
                    id: innerCapsuleShape
                    diameter: 50
                    height: 100
                }
                position: Qt.vector3d(0, (innerCapsuleShape.diameter + innerCapsuleShape.height) / 2, 0)
            }
            mass: 10

            property real characterHeight: capsuleShape.height + capsuleShape.diameter
            property vector3d movement: Qt.vector3d(wasd.sideSpeed, 0, wasd.forwardSpeed)
            Behavior on movement {
                PropertyAnimation { duration: 200 }
            }

            supportingVolume: Qt.vector4d(0, 1, 0, -capsuleShape.diameter / 2)
            innerBodyShapeLayer: moving

            eulerRotation.y: wasd.cameraRotation.x
            PerspectiveCamera {
                id: camera
                position: Qt.vector3d(0, character.characterHeight / 2 - 10, 0)
                eulerRotation.x: wasd.cameraRotation.y
                clipFar: 10000
                clipNear: 10
            }

            onPositionChanged: console.log(position)
        }
    }

    function isSupported() {
        var groundState = character.getGroundState();
        return groundState === CharacterVirtual.OnGround || groundState === CharacterVirtual.OnSteepGround;
    }

    function handleInput(frameDelta) {
        var movementDirection = character.movement;

        var rotation = Quaternion.fromAxisAndAngle(0, 1, 0, character.eulerRotation.y)
        movementDirection = rotation.times(movementDirection);

        var playerControlsHorizontalVelocity = controlMovementDuringJump || isSupported()
        if (playerControlsHorizontalVelocity) {
            desiredVelocity = Qt.vector3d(movementDirection.x, movementDirection.y, movementDirection.z);
        }

        var characterUpRotation = Quaternion.fromEulerAngles(0, 0, 0);
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

        if (building.inGravityField)
            newVelocity = newVelocity.plus(Qt.vector3d(0, 100, 0).times(frameDelta));
        else
            newVelocity = newVelocity.plus(physicsSystem.gravity.times(frameDelta));

        if (playerControlsHorizontalVelocity) {
            newVelocity = newVelocity.plus(characterUpRotation.times(desiredVelocity));
        } else {
            var currentHorizontalVelocity = currentVelocity.minus(currentVerticalVelocity);
            newVelocity = newVelocity.plus(currentHorizontalVelocity);
        }

        character.setLinearVelocity(newVelocity);
    }

    ExtendedUpdateSettings {
        id: updateSettings;
        walkStairsStepUp: Qt.vector3d(0, capsuleShape.height, 0)
    }

    Connections {
        target: physicsSystem
        function onBeforeFrameDone(frameDelta) {
            handleInput(frameDelta);

            character.extendedUpdate(frameDelta,
                                     Qt.vector3d(0, -1, 0).times(physicsSystem.gravity.length()),
                                     updateSettings,
                                     moving,
                                     moving);
        }
    }

    Wasd {
        id: wasd
        property real walkingSpeed: 500
        property real speedFactor: sprintActive ? 3 : crouchActive ? 0.3 : 1
        property real sideSpeed: (moveLeft ? -1 : moveRight ? 1 : 0) * walkingSpeed * speedFactor
        property real forwardSpeed: (moveForwards ? -1 : moveBackwards ? 1 : 0) * walkingSpeed * speedFactor
        cameraRotation.x: 180
    }
}
