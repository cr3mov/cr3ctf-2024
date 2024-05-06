from dataclasses import dataclass, field

from ..const.items import DEFAULT_ITEM, ItemInfo
from ..proto.protocol_pb2 import SAchievementUnlocked
from .wave import Wave


@dataclass
class GameState:
    level: int = 1
    xp_count: int = 0

    total_mobs_killed: int = 0

    wave_number: int = 0
    waves: list[Wave] = field(default_factory=list)

    inventory: list[ItemInfo] = field(default_factory=lambda: [DEFAULT_ITEM])

    unlocked_achievements: list[int] = field(default_factory=list)

    is_cheater: bool = False

    def unlock_achievement(self, session, id_: int, name: str) -> None:
        if id_ in self.unlocked_achievements:
            return

        self.unlocked_achievements.append(id_)
        session.inject_packet(SAchievementUnlocked(
            name=name
        ))
        session.log_info(f'Achievement {len(session.game.unlocked_achievements)} '
                         f'"{name}" has been unlocked')
