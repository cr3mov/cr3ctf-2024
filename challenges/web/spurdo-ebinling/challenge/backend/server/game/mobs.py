from dataclasses import dataclass
from random import randint
from secrets import randbelow

from loguru import logger

from ..const.chances import invincible_mob_chance, mob_drop_chance
from ..const.items import ItemInfo, random_item
from ..const.numbers import MAX_MOBS, MIN_MOBS, MOB_XP, generate_mob_damage, generate_mob_health


@dataclass(frozen=True)
class DroppedItemDetail:
    name: str
    damage: int
    icon: str


@dataclass(frozen=True)
class Mob:
    health: int
    damage: int
    invincible: bool
    xp: int
    dropped_item: DroppedItemDetail | None = None


@dataclass(frozen=True)
class GeneratedEnemies:
    total_xp: int
    enemies: list[Mob]
    contains_invincible_enemies: bool = False


class MobsGenerator:
    @staticmethod
    @logger.catch
    def generate_mobs(wave_number: int) -> GeneratedEnemies:
        result: list[Mob] = list()

        drop_item: ItemInfo | None = None
        if mob_drop_chance():
            drop_item = random_item()

        mobs_count: int = randint(MIN_MOBS, MAX_MOBS)
        mob_with_drop: int = randbelow(mobs_count)
        contains_invincible_mobs: bool = invincible_mob_chance(wave_number)

        for i in range(randint(MIN_MOBS, MAX_MOBS)):
            health: int = generate_mob_health(wave_number)
            damage: int = generate_mob_damage(wave_number)
            xp: int = MOB_XP
            invincible: bool = False

            dropped_item: DroppedItemDetail | None = None
            if mob_with_drop == i:
                if drop_item is not None:
                    dropped_item = DroppedItemDetail(
                        name=drop_item.name,
                        damage=drop_item.damage,
                        icon=drop_item.icon,
                    )
                invincible = contains_invincible_mobs

            if invincible:
                health = damage = int(1e6)

            result.append(Mob(
                health=health,
                damage=damage,
                invincible=invincible,
                xp=xp,
                dropped_item=dropped_item,
            ))

        return GeneratedEnemies(
            enemies=result,
            contains_invincible_enemies=contains_invincible_mobs,
            total_xp=sum(x.xp for x in result),
        )
