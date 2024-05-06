from dataclasses import dataclass
from time import time

from .mobs import GeneratedEnemies, MobsGenerator


@dataclass(frozen=True)
class Wave:
    started_at_user_time: float
    started_at_local_time: float
    mobs: GeneratedEnemies

    @classmethod
    def new(cls, user_time: float, number: int) -> 'Wave':
        return cls(
            started_at_user_time=user_time,
            started_at_local_time=time(),
            mobs=MobsGenerator.generate_mobs(number),
        )
