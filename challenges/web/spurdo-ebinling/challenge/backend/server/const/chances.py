from secrets import randbelow

from loguru import logger


@logger.catch
def roll_chance(value: int | float) -> bool:
    return randbelow(100) <= int(value)


@logger.catch
def invincible_mob_chance(wave_number: int) -> bool:
    return roll_chance(min(int(1 / 5 * wave_number), 100))


@logger.catch
def mob_drop_chance() -> bool:
    return roll_chance(55)
