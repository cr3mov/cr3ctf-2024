// Array to store bullets
const items = [];
function createItem(position, item) {

    const i = {
        item: item,
        mesh: new THREE.Mesh(powerUpGeometry, powerUpMaterial), // Reference to the bullet's mesh
        checkCollision: function(player) {
            if (this.mesh.position.distanceTo(player.mesh.position) < player.radius) {
                return true; // Collision occurred
            }
            return false; // No collision
        },

        pickUp: function() {

            if (!this.checkCollision(player))
                return;

            spurdoWS.pickUp(this.item);
            scene.remove(this.mesh);
        }
    };

    scene.add(i.mesh);

    i.mesh.position.copy({...position, z: 0});
    return i;
}

// Update items and check collisions
function updateItems() {
    // Check bullet-enemy collisions
    items.forEach((item, index) => {

        if (item.checkCollision(player)) {
            item.pickUp();
            items.splice(index, 1);
        }
    });
}