import QtQuick

Item {
    id: root
    
    // Properties from Column that we want to expose
    property int spacing: 0
    property alias children: column.children
    default property alias content: column.data
    
    // Current focus index
    property int currentIndex: 0
    
    // Signal emitted when focus changes
    signal focusChanged(int newIndex)
    
    // Internal Column
    Column {
        id: column
        anchors.fill: parent
        spacing: root.spacing
    }
    
    // Handle key navigation
    Keys.onUpPressed: {
        if (currentIndex > 0) {
            currentIndex--;
            focusChild();
            event.accepted = true;
        }
    }
    
    Keys.onDownPressed: {
        if (currentIndex < column.children.length - 1) {
            currentIndex++;
            focusChild();
            event.accepted = true;
        }
    }
    
    // Focus the child at the current index
    function focusChild() {
        if (currentIndex >= 0 && currentIndex < column.children.length) {
            var child = column.children[currentIndex];
            if (child && child.focus !== undefined) {
                child.forceActiveFocus();
                focusChanged(currentIndex);
            }
        }
    }
    
    // Initialize focus on the first item when this component gets focus
    onFocusChanged: {
        if (focus && column.children.length > 0) {
            focusChild();
        }
    }
}