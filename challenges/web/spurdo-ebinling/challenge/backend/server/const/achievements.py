from dataclasses import dataclass
from typing import Callable

from ..const.items import ITEMS_COUNT
from ..game.state import GameState


@dataclass
class Achievement:
    name: str
    condition: Callable[[GameState], bool]


available_achievements = [
    Achievement(name='kill 1 mob', condition=lambda x: x.total_mobs_killed >= 1),
    Achievement(name='kill 5 mobs', condition=lambda x: x.total_mobs_killed >= 5),
    Achievement(name='kill 100 mobs', condition=lambda x: x.total_mobs_killed >= 100),

    Achievement(name='unlock level 2', condition=lambda x: x.level >= 2),
    Achievement(name='unlock level 5', condition=lambda x: x.level >= 5),
    Achievement(name='unlock level 75', condition=lambda x: x.level >= 75),

    Achievement(name='survive 5 waves', condition=lambda x: x.wave_number >= 5),
    Achievement(name='survive 50 waves', condition=lambda x: x.wave_number >= 50),
    Achievement(name='survive 150 waves', condition=lambda x: x.wave_number >= 150),

    Achievement(name='unlock all items', condition=lambda x: len(x.inventory) >= ITEMS_COUNT),

    Achievement(name='cheater!', condition=lambda x: x.is_cheater),

    # is this enough? if not, please let me know, and we'll make a pt.2 of this challenge :^)
]
