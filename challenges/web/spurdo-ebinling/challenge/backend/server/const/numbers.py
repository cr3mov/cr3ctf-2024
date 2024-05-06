from loguru import logger


MIN_MOBS = 100
MAX_MOBS = 250

MOB_XP = 50

SHOTS_PER_SECOND = 2

WAVE_CLEARANCE_WINDOW = 5
WAVE_REMOTE_CLEARANCE_WINDOW = 125


@logger.catch
def generate_mob_health(wave_number: int) -> int:
    return int(100 * ((1 / 10) * wave_number))


@logger.catch
def generate_mob_damage(wave_number: int) -> int:
    return int(100 * ((1 / 5) * wave_number))


@logger.catch
def xp_required_to_level_up(current_level: int) -> int:
    return 8 * current_level * MOB_XP


@logger.catch
def player_damage_for_level(level: int) -> int:
    return 5 * level


@logger.catch
def min_wave_clearance_time(total_mobs_health: int, current_level: int) -> int:
    return int(total_mobs_health / player_damage_for_level(current_level) / SHOTS_PER_SECOND)
