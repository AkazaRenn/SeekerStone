import QtQuick
import QtQuick.Controls
import QtQuick.Window

import "utils/FocusManager.js" as FocusManager

ApplicationWindow {
    property var focusableItems: []

    Column {
        anchors.centerIn: parent
        spacing: 20

        Button {
            id: playButton
            text: "Play"
        }

        TextField {
            id: nameField
            text: "Enter name"
        }

        Slider {
            id: volumeSlider
            value: 50
        }

        Button {
            id: exitButton
            text: "Exit"
        }
    }

    Component.onCompleted: {
        focusableItems = parent.children.filter(item => item.Focus)
    }

    Keys.onDownPressed: FocusManager.moveFocus(focusableItems, focusItem, "down")
    Keys.onUpPressed: FocusManager.moveFocus(focusableItems, focusItem, "up")
    Keys.onLeftPressed: FocusManager.moveFocus(focusableItems, focusItem, "left")
    Keys.onRightPressed: FocusManager.moveFocus(focusableItems, focusItem, "right")
}
