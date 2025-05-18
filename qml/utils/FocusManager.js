function moveFocus(direction) {
    let currentItem = focusItem; // Currently focused element
    let closestItem = null;
    let closestDistance = Infinity;

    focusableItems.forEach(item => {
        if (item === currentItem) return; // Skip itself

        let dx = item.x - currentItem.x;
        let dy = item.y - currentItem.y;

        if ((direction === "down" && dy > 0) ||
            (direction === "up" && dy < 0) ||
            (direction === "left" && dx < 0) ||
            (direction === "right" && dx > 0)) {

            let distance = Math.sqrt(dx * dx + dy * dy); // Pythagorean distance
            if (distance < closestDistance) {
                closestDistance = distance;
                closestItem = item;
            }
        }
    });

    if (closestItem) {
        closestItem.forceActiveFocus();
    }
}
