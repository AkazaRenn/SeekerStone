import QtQuick
import QtQuick.Controls
import QtQuick.Window

ApplicationWindow {
    visibility: Window.FullScreen
    flags: Qt.FramelessWindowHint

    // Utils.NavigableColumn {
    //     id: mainMenu
    //     anchors.centerIn: parent
    //     spacing: 20
    //     focus: true

    //     Button {
    //         id: playButton
    //         text: "Play"
    //         onClicked: console.log("Play clicked")
    //     }

    //     TextField {
    //         id: nameField
    //         text: "Enter name"
    //     }

    //     Slider {
    //         id: volumeSlider
    //         value: 50
    //     }

    //     Button {
    //         id: exitButton
    //         text: "Exit"
    //         onClicked: Qt.quit()
    //     }

    //     // Example of a navigable row inside the column
    //     Utils.NavigableRow {
    //         id: optionsRow
    //         spacing: 10
    //         height: 40

    //         Button {
    //             text: "Option 1"
    //             onClicked: console.log("Option 1 clicked")
    //         }

    //         Button {
    //             text: "Option 2"
    //             onClicked: console.log("Option 2 clicked")
    //         }

    //         Button {
    //             text: "Option 3"
    //             onClicked: console.log("Option 3 clicked")
    //         }
    //     }
    // }

    Component.onCompleted: {
        Screen.primary.virtualGeometry.width = width
        Screen.primary.virtualGeometry.height = height
    }
}