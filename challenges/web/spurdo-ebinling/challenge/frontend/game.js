const Game = {

    // Function to update the game state
    update: function() {

        if (!this.isNotificationsEnabled())
            return;

        if (spurdoWS.readyState !== 1)
            return;

        if (enemies.length === 0 && upcomingEnemies.length === 0 && !sentWaveEnd) {

            document.spurdoWS.waveEnd(waveBegin, enemiesKilled)
            sentWaveEnd = true;
        }

        let dx = 0, dy = 0;

        if (keys[87]) dy += player.speed; // W key
        if (keys[83]) dy -= player.speed; // S key
        if (keys[65]) dx -= player.speed; // A key
        if (keys[68]) dx += player.speed; // D key

        player.move(dx, dy);

        updateItems();
        updateBullets();

        // Move enemies towards the player
        enemies.forEach((enemy, index) => {
            enemy.moveTowards(player);

            if (enemy.health <= 0) {
                document.killedEnemies.push(enemy);

                if (!!enemy.itemDrop) {
                    items.push(createItem(enemy.mesh.position, enemy.itemDrop));
                }
                player.gainXp(enemy.xpGain);

                enemies.splice(index, 1);
                scene.remove(enemy.mesh);
            }

            // Check collision between player and enemies
            const distance = player.mesh.position.distanceTo(enemy.mesh.position);
            const combinedRadius = player.radius + enemy.radius;

            if (distance < combinedRadius) {
                const overlap = combinedRadius - distance;
                const pushX = (player.mesh.position.x - enemy.mesh.position.x) / distance * overlap / 2;
                const pushY = (player.mesh.position.y - enemy.mesh.position.y) / distance * overlap / 2;

                enemy.move(-pushX, -pushY);
                player.push(pushX, pushY);

                if (!player.invulnerable || enemy.invincible) {
                    player.health -= enemy.damage;
                    if (player.health <= 0) {
                        this.gameOver();
                    }
                    player.invulnerable = true;
                    setTimeout(() => {
                        player.invulnerable = false;
                    }, player.invulnerabilityTime * 1000);
                }
            }
        });

        for (let i = 0; i < enemies.length; i++) {
            for (let j = i + 1; j < enemies.length; j++) {
                const enemy1 = enemies[i];
                const enemy2 = enemies[j];
                if (enemy1.checkCollisionWith(enemy2)) {
                    enemy1.resolveCollisionWith(enemy2);
                }
            }
        }

        // Randomly generate enemies
        if (Math.random() < 0.01 && !!upcomingEnemies.length) {
            generateEnemy();
        }
    },

    gameOver: function() {
        document.isDead = true;
        Game.notify('HUGE LLL', `Wow, look at this looser :DDDDD`);
        spurdoWS.close();
        location.reload();
    },

    reset: function() {
        player.y = player.x = 0;
        player.health = 100;
        player.invulnerable = false;
        enemies.length = 0;

        for( var i = scene.children.length - 1; i >= 0; i--) {
            obj = scene.children[i];
            scene.remove(obj);
        }
        scene.add(mapMesh);

        // Set the position of the map background behind other objects
        mapMesh.position.z = -2;

        clearInterval(player.shootInterval);
        player.shootInterval = setInterval(autoShoot, 500);

        player.createMesh()
    },

    // Function to render the scene
    render: function() {
        requestAnimationFrame(this.render.bind(this));
        this.update();
        renderer.render(scene, camera);
        // Move the camera to follow the player
        camera.position.x = player.mesh.position.x;
        camera.position.y = player.mesh.position.y;
    },

    isNotificationsEnabled: function() {

        if (!('Notification' in window))
            return true;

        if (Notification.permission === 'granted')
            return true;

        Notification.requestPermission().then((permission) => {
            if (permission !== 'granted') {
                let show = function(x) { alert(x); console.error(x); }
                show('Looks like you wont be able to play the game because you are bozo, enable notifications quickly');
            }
        });
    },

    notify: function(title, text) {
        if (!('Notification' in window))
            return;

        return new Notification(title, {body: text});
    }
}