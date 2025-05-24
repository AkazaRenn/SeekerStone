import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

ApplicationWindow {
    visibility: Window.FullScreen
    flags: Qt.FramelessWindowHint

    Column {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        spacing: 10
        
        Button {
            text: "Test 1"
            onClicked: console.log("Test 1 clicked")
        }
        
        Button {
            text: "Test 2"
            onClicked: console.log("Test 2 clicked")
        }
        
        Button {
            text: "Test 3"
            onClicked: console.log("Test 3 clicked")
        }
        
        Button {
            text: "Exit"
            onClicked: Qt.quit()
        }
    }

    Component.onCompleted: {
        Screen.primary.virtualGeometry.width = width
        Screen.primary.virtualGeometry.height = height
    }
}