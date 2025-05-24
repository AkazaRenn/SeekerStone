import QtQuick

Item {
    id: rowRoot
    
    // Properties from Row that we want to expose
    property int spacing: 0
    property alias children: row.children
    default property alias content: row.data
    
    // Current focus index
    property int currentIndex: 0
    
    // Signal emitted when focus changes
    signal focusChanged(int newIndex)
    
    // Internal Row
    Row {
        id: row
        anchors.fill: parent
        spacing: rowRoot.spacing
    }
    
    // Handle key navigation
    Keys.onLeftPressed: {
        if (currentIndex > 0) {
            currentIndex--;
            focusChild();
            event.accepted = true;
        }
    }
    
    Keys.onRightPressed: {
        if (currentIndex < row.children.length - 1) {
            currentIndex++;
            focusChild();
            event.accepted = true;
        }
    }
    
    // Focus the child at the current index
    function focusChild() {
        if (currentIndex >= 0 && currentIndex < row.children.length) {
            var child = row.children[currentIndex];
            if (child && child.focus !== undefined) {
                child.forceActiveFocus();
                focusChanged(currentIndex);
            }
        }
    }
    
    // Initialize focus on the first item when this component gets focus
    onFocusChanged: {
        if (focus && row.children.length > 0) {
            focusChild();
        }
    }
}