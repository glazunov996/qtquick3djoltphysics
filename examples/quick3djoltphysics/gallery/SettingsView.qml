import QtQuick
import QtQuick.Controls
import QtQuick3D.JoltPhysics

Item {
    id: rootItem

    width: settingsDrawer.width
    height: settingsDrawer.height

    property int maxConcurrentJobs
    property int gravity
    property int collisionSteps
    property int numVelocitySteps
    property int numPositionSteps
    property real baumgarte
    property real speculativeContactDistance
    property real penetrationSlop
    property real linearCastThreshold
    property real minVelocityForRestitution
    property real timeBeforeSleep
    property real pointVelocitySleepThreshold
    property bool deterministicSimulation
    property bool constraintWarmStart
    property bool useBodyPairContactCache
    property bool useManifoldReduction
    property bool useLargeIslandSplitter
    property bool allowSleeping
    property bool checkActiveEdges

    Text {
        x: Window.window.width - implicitWidth - 10
        y: 10
        text: AppSettings.showSettingsView ? "ESC: Menu" : (AppSettings.playPhysics ? "P: Pause, R: Restart, ESC: Menu" : "P: Unpause, R: Restart, ESC: Menu")
        color: "#f0f0f0"
        font.pointSize: AppSettings.fontSizeLarge
    }

    Drawer {
        id: settingsDrawer
        modal: false
        edge: Qt.RightEdge
        interactive: false
        y: 64
        leftInset: -10
        topInset: -20
        bottomInset: -20
        topMargin: 10
        visible: AppSettings.showSettingsView

        background: Rectangle {
            color: "#80404040"
            border.color: "#000000"
            border.width: 1
            opacity: 0.8
        }

        onAboutToShow: {
            maxConcurrentJobs = AppSettings.maxConcurrentJobs
            gravity = AppSettings.gravity
            collisionSteps = AppSettings.collisionSteps
            numVelocitySteps = AppSettings.numVelocitySteps
            numPositionSteps = AppSettings.numPositionSteps
            baumgarte = AppSettings.baumgarte
            speculativeContactDistance = AppSettings.speculativeContactDistance
            penetrationSlop = AppSettings.penetrationSlop
            linearCastThreshold = AppSettings.linearCastThreshold
            minVelocityForRestitution = AppSettings.minVelocityForRestitution
            timeBeforeSleep = AppSettings.timeBeforeSleep
            pointVelocitySleepThreshold = AppSettings.pointVelocitySleepThreshold
            deterministicSimulation = AppSettings.deterministicSimulation
            constraintWarmStart = AppSettings.constraintWarmStart
            useBodyPairContactCache = AppSettings.useBodyPairContactCache
            useManifoldReduction = AppSettings.useManifoldReduction
            useLargeIslandSplitter = AppSettings.useLargeIslandSplitter
            allowSleeping = AppSettings.allowSleeping
            checkActiveEdges = AppSettings.checkActiveEdges
        }

        Column {
            CustomLabel {
                text: "Max Concurrent Jobs"
            }

            CustomSlider {
                sliderValue: maxConcurrentJobs
                fromValue: 0
                toValue: 16
                sliderStepSize: 1
                onSliderValueChanged: {
                    maxConcurrentJobs = sliderValue
                }
            }

            CustomLabel {
                text: "Gravity (m/s^2)"
            }

            CustomSlider {
                sliderValue: gravity
                fromValue: 0
                toValue: 20
                sliderStepSize: 1
                onSliderValueChanged: {
                    gravity = sliderValue
                }
            }

            CustomLabel {
                text: "Num Collision Steps"
            }

            CustomSlider {
                sliderValue: collisionSteps
                fromValue: 1
                toValue: 4
                sliderStepSize: 1
                onSliderValueChanged: {
                    collisionSteps = sliderValue
                }
            }

            CustomLabel {
                text: "Num Velocity Steps"
            }

            CustomSlider {
                sliderValue: numVelocitySteps
                fromValue: 0
                toValue: 30
                sliderStepSize: 1
                onSliderValueChanged: {
                    numVelocitySteps = sliderValue
                }
            }

            CustomLabel {
                text: "Num Position Steps"
            }

            CustomSlider {
                sliderValue: numPositionSteps
                fromValue: 0
                toValue: 30
                sliderStepSize: 1
                onSliderValueChanged: {
                    numPositionSteps = sliderValue
                }
            }

            CustomLabel {
                text: "Baumgarte Stabilization Factor"
            }

            CustomSlider {
                sliderValue: baumgarte
                fromValue: 0.01
                toValue: 1
                sliderStepSize: 0.05
                onSliderValueChanged: {
                    baumgarte = sliderValue
                }
            }

            CustomLabel {
                text: "Speculative Contact Distance (m)"
            }

            CustomSlider {
                sliderValue: speculativeContactDistance
                fromValue: 0
                toValue: 0.1
                sliderStepSize: 0.005
                onSliderValueChanged: {
                    speculativeContactDistance = sliderValue
                }
            }

            CustomLabel {
                text: "Penetration Slop (m)"
            }

            CustomSlider {
                sliderValue: penetrationSlop
                fromValue: 0
                toValue: 0.1
                sliderStepSize: 0.005
                onSliderValueChanged: {
                    penetrationSlop = sliderValue
                }
            }

            CustomLabel {
                text: "Linear Cast Threshold"
            }

            CustomSlider {
                sliderValue: linearCastThreshold
                fromValue: 0
                toValue: 1
                sliderStepSize: 0.05
                onSliderValueChanged: {
                    linearCastThreshold = sliderValue
                }
            }

            CustomLabel {
                text: "Min Velocity For Restitution (m/s)"
            }

            CustomSlider {
                sliderValue: minVelocityForRestitution
                fromValue: 0
                toValue: 10
                sliderStepSize: 0.1
                onSliderValueChanged: {
                    minVelocityForRestitution = sliderValue
                }
            }

            CustomLabel {
                text: "Time Before Sleep (s)"
            }

            CustomSlider {
                sliderValue: timeBeforeSleep
                fromValue: 0.1
                toValue: 1
                sliderStepSize: 0.1
                onSliderValueChanged: {
                    timeBeforeSleep = sliderValue
                }
            }

            CustomLabel {
                text: "Point Velocity Sleep Threshold (m/s)"
            }

            CustomSlider {
                sliderValue: pointVelocitySleepThreshold
                fromValue: 0.01
                toValue: 1
                sliderStepSize: 0.01
                onSliderValueChanged: {
                    pointVelocitySleepThreshold = sliderValue
                }
            }

            CustomCheckBox {
                text: "Deterministic Simulation"
                checked: deterministicSimulation
                onCheckedChanged: deterministicSimulation = checked
            }

            CustomCheckBox {
                text: "Constraint Warm Starting"
                checked: constraintWarmStart
                onCheckedChanged: constraintWarmStart = checked
            }

            CustomCheckBox {
                text: "Use Body Pair Contact Cache"
                checked: useBodyPairContactCache
                onCheckedChanged: useBodyPairContactCache = checked
            }

            CustomCheckBox {
                text: "Contact Manifold Reduction"
                checked: useManifoldReduction
                onCheckedChanged: useManifoldReduction = checked
            }

            CustomCheckBox {
                text: "Use Large Island Splitter"
                checked: useLargeIslandSplitter
                onCheckedChanged: useLargeIslandSplitter = checked
            }

            CustomCheckBox {
                text: "Allow Sleeping"
                checked: allowSleeping
                onCheckedChanged: allowSleeping = checked
            }

            CustomCheckBox {
                text: "Check Active Triangle Edges"
                checked: checkActiveEdges
                onCheckedChanged: checkActiveEdges = checked
            }

            Row {
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    text: qsTr("Apply Changes")
                    font.pointSize: AppSettings.fontSizeSmall
                    onClicked: {
                        AppSettings.maxConcurrentJobs = maxConcurrentJobs
                        AppSettings.gravity = gravity
                        AppSettings.collisionSteps = collisionSteps
                        AppSettings.numVelocitySteps = numVelocitySteps
                        AppSettings.numPositionSteps = numPositionSteps
                        AppSettings.baumgarte = baumgarte
                        AppSettings.speculativeContactDistance = speculativeContactDistance
                        AppSettings.penetrationSlop = penetrationSlop
                        AppSettings.linearCastThreshold = linearCastThreshold
                        AppSettings.minVelocityForRestitution = minVelocityForRestitution
                        AppSettings.timeBeforeSleep = timeBeforeSleep
                        AppSettings.pointVelocitySleepThreshold = pointVelocitySleepThreshold
                        AppSettings.deterministicSimulation = deterministicSimulation
                        AppSettings.constraintWarmStart = constraintWarmStart
                        AppSettings.useBodyPairContactCache = useBodyPairContactCache
                        AppSettings.useManifoldReduction = useManifoldReduction
                        AppSettings.useLargeIslandSplitter = useLargeIslandSplitter
                        AppSettings.allowSleeping = allowSleeping
                        AppSettings.checkActiveEdges = checkActiveEdges
                        AppSettings.testRestartNeeded()
                    }
                }
            }
        }

        enter: Transition {
            NumberAnimation {
                property: "position"
                to: 1.0
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }

        exit: Transition {
            NumberAnimation {
                property: "position"
                to: 0.0
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    }
}
