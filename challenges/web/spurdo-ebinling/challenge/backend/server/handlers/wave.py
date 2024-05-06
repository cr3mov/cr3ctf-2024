from time import time

from ..const.achievements import available_achievements
from ..const.numbers import WAVE_CLEARANCE_WINDOW, WAVE_REMOTE_CLEARANCE_WINDOW, min_wave_clearance_time
from ..core.config import config
from ..core.handlers import MessageHandler
from ..core.session import Session
from ..enums.packet_type import EPacketType
from ..game.wave import Wave as LocalWave
from ..proto.protocol_pb2 import ItemDetail, SAchievementUnlocked, Wave


handler = MessageHandler(EPacketType.WAVE)


@handler.dispatcher()
async def on_wave(session: Session, message: Wave) -> Wave:
    cleared_in: float = .0
    min_clearance_time: float = .0

    if session.game.wave_number > 0:
        if message.number > len(session.game.waves):
            return message

        wave_info = session.game.waves[message.number - 1]

        if len(wave_info.mobs.enemies) != len(message.mobs):
            session.log_critical('Attempted to send invalid wave end message. len(enemies) != mobs_killed')

        if wave_info.mobs.contains_invincible_enemies:
            session.log_critical('Attempted to clear wave with an invincible mob!')

        cleared_in = message.current_time - wave_info.started_at_user_time
        min_clearance_time = (min_wave_clearance_time(
            sum([x.health for x in wave_info.mobs.enemies]), session.game.level
        ) * 1e3)
        min_clearance_time -= WAVE_REMOTE_CLEARANCE_WINDOW

        if cleared_in < min_clearance_time:
            session.log_critical(f'Cleared wave too quickly ({cleared_in} vs {min_clearance_time} diff '
                                 f'{min_clearance_time - cleared_in})')

        cleared_in_local: float = time() - wave_info.started_at_local_time
        cleared_in_local -= WAVE_CLEARANCE_WINDOW
        cleared_in_local *= 1e3
        if cleared_in_local > cleared_in:
            session.log_critical(f'Cleared wave too quickly #2 ({cleared_in_local} vs {cleared_in})')

        session.game.total_mobs_killed += len(wave_info.mobs.enemies)

    if session.game.wave_number > 0:
        session.log_info(f'Cleared wave in {cleared_in:.0f} (best {min_clearance_time:.0f} diff '
                         f'{(cleared_in - min_clearance_time):.0f}). Starting a new one.')

    for i, achievement in enumerate(available_achievements):
        if i in session.game.unlocked_achievements:
            continue

        if achievement.condition(session.game) is False:
            continue

        session.game.unlock_achievement(session, i, achievement.name)

    if len(session.game.unlocked_achievements) == len(available_achievements):
        session.inject_packet(SAchievementUnlocked(
            name=config.FLAG
        ))

    session.game.wave_number += 1
    new_wave = LocalWave.new(message.current_time, session.game.wave_number)
    session.game.waves.append(new_wave)

    session.game.xp_count += new_wave.mobs.total_xp

    return Wave(
        number=session.game.wave_number,
        mobs=[
            Wave.Mob(
                health=mob.health,
                damage=mob.damage,
                invincible=mob.invincible,
                xp=mob.xp,
                drop=ItemDetail(
                    name=mob.dropped_item.name,
                    damage=mob.dropped_item.damage,
                    icon=mob.dropped_item.icon,
                ) if mob.dropped_item is not None else None
            )
            for mob in new_wave.mobs.enemies
        ],
        current_time=message.current_time,
    )
