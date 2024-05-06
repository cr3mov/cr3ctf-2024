from enum import IntEnum, unique


@unique
class EPacketType(IntEnum):
    PING = 0
    WAVE = 1
    INVENTORY = 2
    INVENTORY_EQUIP = 3
    LEVEL_UP = 4
    ACHIEVEMENT_UNLOCKED = 5
    RESOURCE = 6
