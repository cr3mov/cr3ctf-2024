// Array to store bullets
const bullets = [];

document.killedEnemies = [];

function createBullet(position, velocity, damage) {
    let vel = {...velocity, z: 0};
    vel.x *= BULLET_SPEED;
    vel.y *= BULLET_SPEED;

    const bullet = {
        velocity: vel,
        damage: damage,
        mesh: new THREE.Mesh(bulletGeometry, bulletMaterial), // Reference to the bullet's mesh
        move: function() {
            if (this.mesh)
                this.mesh.position.add(this.velocity);
        },
        checkCollision: function(enemy) {
            if (this.mesh.position.distanceTo(enemy.mesh.position) < enemy.radius) {
                return true; // Collision occurred
            }
            return false; // No collision
        }
    };

    scene.add(bullet.mesh);

    bullet.mesh.position.copy({...position});
    bullet.mesh.rotation.z = Math.atan2(vel.y, vel.x) + Math.PI / 2;
    return bullet;
}

function autoShoot() {
    let closestEnemy = null;
    let closestDistance = Infinity;

    // Find the closest enemy to the player
    enemies.forEach(enemy => {
        const distance = player.mesh.position.distanceTo(enemy.mesh.position);
        if (distance < closestDistance) {
            closestDistance = distance;
            closestEnemy = enemy;
        }
    });

    // If there is a closest enemy, shoot a bullet towards it
    if (closestEnemy) {
        const direction = new THREE.Vector2().subVectors(closestEnemy.mesh.position, player.mesh.position).normalize();
        bullets.push(createBullet(player.mesh.position, direction, player.damage));
    }
}

// Update function to move bullets and check collisions
function updateBullets() {
    // Check bullet-enemy collisions
    bullets.forEach((bullet, index) => {
        bullet.move();

        if (bullet.mesh.position.x - player.mesh.position.x > window.innerWidth ||
            bullet.mesh.position.y - player.mesh.position.y > window.innerHeight ||
            bullet.mesh.position.x - player.mesh.position.x < -window.innerWidth ||
            bullet.mesh.position.y - player.mesh.position.y < -window.innerHeight
        ) {
            scene.remove(bullet.mesh);
            bullets.splice(index, 1);
        }


        enemies.forEach(enemy => {
            if (bullet.checkCollision(enemy)) {
                // Apply damage to the enemy
                enemy.health -= bullet.damage;

                // Remove bullet from the scene
                scene.remove(bullet.mesh);
                bullets.splice(index, 1);
            }
        });
    });
}