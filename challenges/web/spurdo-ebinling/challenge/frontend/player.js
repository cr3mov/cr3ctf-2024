const player = {
    mesh: null,
    submesh: null,
    radius: PLAYER_RADIUS,
    speed: PLAYER_SPEED,
    health: 100,
    invulnerable: false,
    invulnerabilityTime: 0.3,
    shootInterval: null,
    prevRotationVelocity: null,
    xpToLvlUp: 400,
    damage: BULLET_DAMAGE,
    curXp: 0,
    curLvl: 1,

    calcNextLvlXp: function () {
        return 8 * this.curLvl * 50;
    },

    gainXp: function(xp) {
        this.curXp += xp;

        const lvl = this.curLvl;
        while (this.curXp >= this.xpToLvlUp) {
            this.curXp -= this.xpToLvlUp;
            ++this.curLvl;
            this.xpToLvlUp = this.calcNextLvlXp();
            Game.notify('Level Up!', `Congrats, now your level is ${this.curLvl}!`);
        }

        this.damage = BULLET_DAMAGE * this.curLvl;

        if (lvl !== this.curLvl)
            spurdoWS.levelUp(this.curLvl);
    },

    createMesh: function() {
        const geometry = new THREE.CircleGeometry(this.radius - 8, 32);
        const material = new THREE.MeshBasicMaterial({ color: 0x010101 });
        this.mesh = new THREE.Mesh(playerGeometry, playerMaterial);
        this.submesh = new THREE.Mesh(geometry, material);
        this.submesh.position.z = -1;
        scene.add(this.mesh);
        scene.add(this.submesh);
    },
    move: function(dx, dy) {
        this.push(dx, dy);
        this.updateTexture({x:dx, y: dy});
    },

    push: function(dx, dy) {
        this.mesh.position.x += dx;
        this.mesh.position.y += dy;
        this.submesh.position.x += dx;
        this.submesh.position.y += dy;
    },

    updateTexture: function(velocity) {
        if (!velocity.x && !velocity.y)
            return;


        if (!!this.prevRotationVelocity &&
            this.prevRotationVelocity.x === velocity.x &&
            this.prevRotationVelocity.y === velocity.y)
            return;

        if (!this.prevRotationVelocity && (velocity.x > 0 || velocity.y > 0)) {
            this.prevRotationVelocity = { ...velocity };
            return;
        }

        if (!this.prevRotationVelocity) {
            this.prevRotationVelocity = {x: 0, y: 0}
        }

        // Update the UV coordinates of the player's geometry to apply the rotation
        const uvAttribute = this.mesh.geometry.getAttribute('uv');
        const uvs = uvAttribute.array;

        for (let i = 0; i < uvs.length; i += 2) {
            // Get the original UV coordinates
            const u = uvs[i];
            const v = uvs[i + 1];

            if (velocity.x)
                uvs[i] = velocity.x !== this.prevRotationVelocity.x ? 1 - u : u;

            if (velocity.y)
                uvs[i + 1] = velocity.y !== this.prevRotationVelocity.y ? 1 - v : v;

        }

        this.prevRotationVelocity = { ...velocity };
        // Mark the UV attribute as needing an update
        uvAttribute.needsUpdate = true;
    }
};