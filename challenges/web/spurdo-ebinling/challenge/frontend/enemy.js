// Define enemy object
function Enemy() {

    const data = upcomingEnemies[0];
    if(!data.getInvincible())
        upcomingEnemies = upcomingEnemies.slice(1);

    this.mesh = null;
    this.radius = ENEMY_RADIUS;
    this.speed = ENEMY_SPEED;
    this.health = data.getHealth();
    this.damage = data.getDamage();
    this.invincible = data.getInvincible();
    this.itemDrop = data.getDrop();
    this.xpGain = data.getXp();
    this.isInverted = false;
    this.createMesh = function() {
        this.mesh = new THREE.Mesh(enemyGeometry, enemyMaterial);
        scene.add(this.mesh);
    };


    this.move = function(dx, dy) {
        this.mesh.position.x += dx;
        this.mesh.position.y += dy;
    }

    this.moveTowards = function(target) {
        const dx = target.mesh.position.x - this.mesh.position.x;
        const dy = target.mesh.position.y - this.mesh.position.y;
        const angle = Math.atan2(dy, dx);
        const moveX = Math.cos(angle) * this.speed;
        const moveY = Math.sin(angle) * this.speed;
        this.move(moveX, moveY);
        this.updateTexture(Math.sign(moveX));
    };

    this.updateTexture = function(velocity) {

        if (!(velocity > 1e-3 || velocity < -1e-3 ))
            return;

        if (velocity > 1e-3 && !this.isInverted)
            return;

        if (velocity < -1e-3 && this.isInverted)
            return;

        const uvAttribute = this.mesh.geometry.getAttribute('uv');
        const uvs = uvAttribute.array;

        for (let i = 0; i < uvs.length; i += 2) {
            // Get the original UV coordinates
            const u = uvs[i];
            uvs[i] = 1 - u;
        }

        this.isInverted = velocity < -1e-3;
        // Mark the UV attribute as needing an update
        uvAttribute.needsUpdate = true;
    }

    this.checkCollisionWith = function(entity) {
        const distance = this.mesh.position.distanceTo(entity.mesh.position);
        const combinedRadius = this.radius + entity.radius;
        return distance < combinedRadius;
    }

    this.resolveCollisionWith = function(entity) {

        if(entity.invincible || this.invincible)
            return;

        const dx = this.mesh.position.x - entity.mesh.position.x;
        const dy = this.mesh.position.y - entity.mesh.position.y;
        const distance = this.mesh.position.distanceTo(entity.mesh.position);
        const combinedRadius = this.radius + entity.radius;
        const overlap = combinedRadius - distance;
        const pushX = (dx / distance) * overlap / 2;
        const pushY = (dy / distance) * overlap / 2;
        this.move(pushX / 2, pushY / 2);
        entity.move(-pushX / 2, -pushY / 2);
    }
}
// Array to store enemy objects
const enemies = [];
// Function to generate random enemies
function generateEnemy() {
    const enemy = new Enemy();
    enemy.createMesh();
    enemy.mesh.position.x = Math.random() * (window.innerWidth - 30) - (window.innerWidth / 2) + player.mesh.position.x;
    enemy.mesh.position.y = Math.random() * (window.innerHeight - 30) - (window.innerHeight / 2) + player.mesh.position.y;
    enemies.push(enemy);
}