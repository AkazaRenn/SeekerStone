import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    visibility: Window.FullScreen
    flags: Qt.FramelessWindowHint

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainContent
        // pushEnter: null
        // pushExit: null
        // popEnter: null
        // popExit: null
    }

    Rectangle {
        id: mainContent
        width: parent.width
        height: parent.height
        color: "black"

        Column {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 20
            spacing: 10

            Button {
                id: firstButton
                focus: true
                text: "Test 1"
                onClicked: {
                    console.log("Test 1 clicked");
                    stackView.push("LoadingPage.qml");
                    link.start();
                }

                Keys.onEnterPressed: clicked()
                Keys.onReturnPressed: clicked()
                Keys.onSpacePressed: function (event) {
                    // Prevent default space key behavior
                    event.accepted = true;
                }
            }

            Button {
                text: "Exit"
                onClicked: Qt.quit()

                Keys.onEnterPressed: clicked()
                Keys.onReturnPressed: clicked()
                Keys.onSpacePressed: function (event) {
                    // Prevent default space key behavior
                    event.accepted = true;
                }
            }
        }
    }

    Connections {
        target: link
        function finished() {
            stackView.pop();
        }
    }
}
