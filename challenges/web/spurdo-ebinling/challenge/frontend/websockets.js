const protobuf = document.protobuf;
const rtrim = (string, ch) => {
    let end = string.length - 1;
    while (ch.indexOf(string[end]) >= 0) {
        end -= 1;
    }
    return string.substr(0, end + 1);
};

const spurdoWS = new WebSocket(
    rtrim(window.location.toString(), '/')
        .replace("http://", "ws://")
        .replace("https://", "wss://")
    + "/ws"
);
document.spurdoWS = spurdoWS;
document.isDead = false;

document.waveNumber = 0;

spurdoWS.sendMessage = function(type, val) {
    const msg = new protobuf.Packet();
    msg.setType(type);
    msg.setValue(val.serializeBinary());

    const messageBytes = msg.serializeBinary();
    const byteArray = Uint8Array.from([
        0x6d, 0x65, 0x6d, 0x65,
        msg.getType() & 0xff, (msg.getType() >> 8) & 0xff,
        ...messageBytes
    ]);

    this.send(byteArray);
}

spurdoWS.onopen = function(e) {
    this.sendMessage(protobuf.PacketId.PACKET_ID_PING,
        new protobuf.CPing());
}

spurdoWS.onmessage = async function(event) {
    const data =  new Uint8Array((await event.data.arrayBuffer()).slice(6));
    const msg = protobuf.Packet.deserializeBinary(data);

    switch (msg.getType()) {
        case protobuf.PacketId.PACKET_ID_PING: {
            break;
        }
        case protobuf.PacketId.PACKET_ID_WAVE: {
            const value = protobuf.Wave.deserializeBinary(msg.getValue());

            document.killedEnemies = [];
            document.waveNumber = value.getNumber();
            upcomingEnemies = value.getMobsList();
            enemiesKilled = 0;
            sentWaveEnd = false;
            break;
        }
        case protobuf.PacketId.PACKET_ID_INVENTORY: {
            const value = protobuf.SInventory.deserializeBinary(msg.getValue());
            break;
        }
        case protobuf.PacketId.PACKET_ID_INVENTORY_EQUIP: {
            const value = protobuf.SInventoryEquip.deserializeBinary(msg.getValue());
            break;
        }
        case protobuf.PacketId.PACKET_ID_LEVEL_UP: {
            const value = protobuf.SLevelUp.deserializeBinary(msg.getValue());
            break;
        }
        case protobuf.PacketId.PACKET_ID_ACHIEVEMENT_UNLOCKED: {
            const value = protobuf.SAchievementUnlocked.deserializeBinary(msg.getValue());
            Game.notify('Achievement unlocked!', value.getName());
            break;
        }
    }
}

spurdoWS.pickUp = function(item) {
    const pickUp = new protobuf.CInventoryEquip();
    pickUp.setItem(item);

    this.sendMessage(protobuf.PacketId.PACKET_ID_INVENTORY_EQUIP, pickUp);
}

spurdoWS.levelUp = function(level) {
    const lvlUp = new protobuf.CLevelUp();
    lvlUp.setItem(level);

    this.sendMessage(protobuf.PacketId.PACKET_ID_LEVEL_UP, lvlUp);
}

spurdoWS.waveEnd = function(waveBegin, killed) {
    const message = new protobuf.Wave();
    message.setNumber(document.waveNumber);

    const mobs_list = [];
    for(let i = 0; i < document.killedEnemies.length; ++i) {
        const mob = new protobuf.Wave.Mob();
        mob.setDamage(document.killedEnemies[i].damage || 1);
        mob.setDrop(undefined);
        mob.setXp(document.killedEnemies[i].xp || 1);
        mob.setHealth(document.killedEnemies[i].health > 0 ? document.killedEnemies[i].health : 1);
        mob.setInvincible(document.killedEnemies[i].invincible);
        mobs_list.push(mob);
    }

    message.setMobsList(mobs_list);
    message.setCurrentTime(+new Date());

    this.sendMessage(protobuf.PacketId.PACKET_ID_WAVE, message);
}

spurdoWS.onclose = function(event) {
    console.log(event);
    if (!document.isDead)
        alert("Server is cooked! Check out your console logs.");
    spurdoWS.close();
    location.reload();
}

spurdoWS.onerror = function(error) {
    console.error(error);
};
