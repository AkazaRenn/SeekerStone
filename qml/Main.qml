import QtQuick
import QtQuick.Controls
import QtQuick.Window

ApplicationWindow {
    visibility: Window.FullScreen
    flags: Qt.FramelessWindowHint

    Component.onCompleted: {
        Screen.primary.virtualGeometry.width = width
        Screen.primary.virtualGeometry.height = height
    }
}