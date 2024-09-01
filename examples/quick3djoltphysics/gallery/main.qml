// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick
import QtQuick.Controls
import QtQuick3D

Window {
    id: rootWindow
    width: 1280
    height: 960
    visible: true
    title: qsTr("Qt Quick 3D Jolt Physics - Gallery")
    color: "#191970"

    readonly property url startupView: "qrc:/StartupView.qml"

    property var colors: [
        "#ff0000",   // Red
        "#cc8f66",   // Tan
        "#e2f200",   // Yellow
        "#29a67c",   // Green
        "#00aaff",   // Blue
        "#452699",   // Purple
        "#993a82",   // Magenta
        "#e53950",   // Salmon
        "#cc0000",   // Red
        "#ffaa00",   // Orange
        "#558000",   // Olive
        "#40ffd9",   // Turquoise
        "#004b8c",   // Dark Blue
        "#a173e6",   // Lavender
        "#f23d9d",   // Pink
        "#b26559",   // Brown
        "#8c5e00",   // Dark Orange
        "#b5d96c",   // Light Green
        "#40f2ff",   // Light Blue
        "#4d7599",   // Slate Blue
        "#9d3df2",   // Violet
        "#8c0038",   // Maroon
        "#7f3920",   // Sienna
        "#ccad33",   // Gold
        "#40ff40",   // Lime Green
        "#269199",   // Teal
        "#0066ff",   // Azure
        "#f200e2",   // Fuchsia
        "#994d6b",   // Raspberry
        "#e55c00",   // Dark Salmon
        "#8c7e46",   // Khaki
        "#00b347",   // Spring Green
        "#00c2f2",   // Sky Blue
        "#1b00cc",   // Indigo
        "#e673de",   // Orchid
        "#7f0011"    // Dark Red
    ]

    Component.onCompleted: {
        AppSettings.iconSize = Qt.binding(function() {
            return 16 + Math.max(rootWindow.width, rootWindow.height) * 0.05
        })
        backButton.opacity = Qt.binding(function() { return loader.source !== rootWindow.startupView })
    }

    Loader {
        id: loader
        anchors.fill: parent
        Component.onCompleted: setSource(rootWindow.startupView, {loader: loader})
    }

    Button {
        id: backButton
        anchors.left: parent.left
        anchors.top: parent.top
        implicitWidth: AppSettings.iconSize
        implicitHeight: AppSettings.iconSize
        height: width
        opacity: 0;
        visible: opacity
        icon.source: "qrc:/images/arrow_icon.png"
        icon.width: backButton.width * 0.3
        icon.height: backButton.height * 0.3
        icon.color: "transparent"
        background: Rectangle {
            color: "transparent"
        }
        onClicked: {
            AppSettings.playPhysics = false;
            loader.setSource(rootWindow.startupView, {loader: loader})
        }
        Behavior on opacity {
            NumberAnimation {
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    }

    Connections {
        target: AppSettings
        function onTestRestartNeeded() {
            loader.setSource(loader.source)
        }
    }
}
