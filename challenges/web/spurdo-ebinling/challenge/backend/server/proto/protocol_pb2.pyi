from typing import ClassVar as _ClassVar
from typing import Iterable as _Iterable
from typing import Mapping as _Mapping
from typing import Optional as _Optional
from typing import Union as _Union

from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper

DESCRIPTOR: _descriptor.FileDescriptor

class PacketId(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    PACKET_ID_PING: _ClassVar[PacketId]
    PACKET_ID_WAVE: _ClassVar[PacketId]
    PACKET_ID_INVENTORY: _ClassVar[PacketId]
    PACKET_ID_INVENTORY_EQUIP: _ClassVar[PacketId]
    PACKET_ID_LEVEL_UP: _ClassVar[PacketId]
    PACKET_ID_ACHIEVEMENT_UNLOCKED: _ClassVar[PacketId]
    PACKET_ID_REQUEST_RESOURCE: _ClassVar[PacketId]
PACKET_ID_PING: PacketId
PACKET_ID_WAVE: PacketId
PACKET_ID_INVENTORY: PacketId
PACKET_ID_INVENTORY_EQUIP: PacketId
PACKET_ID_LEVEL_UP: PacketId
PACKET_ID_ACHIEVEMENT_UNLOCKED: PacketId
PACKET_ID_REQUEST_RESOURCE: PacketId

class ItemDetail(_message.Message):
    __slots__ = ("name", "damage", "icon")
    NAME_FIELD_NUMBER: _ClassVar[int]
    DAMAGE_FIELD_NUMBER: _ClassVar[int]
    ICON_FIELD_NUMBER: _ClassVar[int]
    name: str
    damage: int
    icon: str
    def __init__(self, name: _Optional[str] = ..., damage: _Optional[int] = ..., icon: _Optional[str] = ...) -> None: ...

class Wave(_message.Message):
    __slots__ = ("number", "mobs", "current_time")
    class Mob(_message.Message):
        __slots__ = ("health", "damage", "invincible", "xp", "drop")
        HEALTH_FIELD_NUMBER: _ClassVar[int]
        DAMAGE_FIELD_NUMBER: _ClassVar[int]
        INVINCIBLE_FIELD_NUMBER: _ClassVar[int]
        XP_FIELD_NUMBER: _ClassVar[int]
        DROP_FIELD_NUMBER: _ClassVar[int]
        health: int
        damage: int
        invincible: bool
        xp: int
        drop: ItemDetail
        def __init__(self, health: _Optional[int] = ..., damage: _Optional[int] = ..., invincible: bool = ..., xp: _Optional[int] = ..., drop: _Optional[_Union[ItemDetail, _Mapping]] = ...) -> None: ...
    NUMBER_FIELD_NUMBER: _ClassVar[int]
    MOBS_FIELD_NUMBER: _ClassVar[int]
    CURRENT_TIME_FIELD_NUMBER: _ClassVar[int]
    number: int
    mobs: _containers.RepeatedCompositeFieldContainer[Wave.Mob]
    current_time: float
    def __init__(self, number: _Optional[int] = ..., mobs: _Optional[_Iterable[_Union[Wave.Mob, _Mapping]]] = ..., current_time: _Optional[float] = ...) -> None: ...

class CPing(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class SPing(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class CInventory(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class SInventory(_message.Message):
    __slots__ = ("items",)
    class Item(_message.Message):
        __slots__ = ("equipped", "detail")
        EQUIPPED_FIELD_NUMBER: _ClassVar[int]
        DETAIL_FIELD_NUMBER: _ClassVar[int]
        equipped: bool
        detail: ItemDetail
        def __init__(self, equipped: bool = ..., detail: _Optional[_Union[ItemDetail, _Mapping]] = ...) -> None: ...
    ITEMS_FIELD_NUMBER: _ClassVar[int]
    items: _containers.RepeatedCompositeFieldContainer[SInventory.Item]
    def __init__(self, items: _Optional[_Iterable[_Union[SInventory.Item, _Mapping]]] = ...) -> None: ...

class CInventoryEquip(_message.Message):
    __slots__ = ("item",)
    ITEM_FIELD_NUMBER: _ClassVar[int]
    item: ItemDetail
    def __init__(self, item: _Optional[_Union[ItemDetail, _Mapping]] = ...) -> None: ...

class SInventoryEquip(_message.Message):
    __slots__ = ("success",)
    SUCCESS_FIELD_NUMBER: _ClassVar[int]
    success: bool
    def __init__(self, success: bool = ...) -> None: ...

class CLevelUp(_message.Message):
    __slots__ = ("item",)
    ITEM_FIELD_NUMBER: _ClassVar[int]
    item: int
    def __init__(self, item: _Optional[int] = ...) -> None: ...

class SLevelUp(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class SAchievementUnlocked(_message.Message):
    __slots__ = ("name",)
    NAME_FIELD_NUMBER: _ClassVar[int]
    name: str
    def __init__(self, name: _Optional[str] = ...) -> None: ...

class CRequestResource(_message.Message):
    __slots__ = ("item",)
    ITEM_FIELD_NUMBER: _ClassVar[int]
    item: str
    def __init__(self, item: _Optional[str] = ...) -> None: ...

class SRequestResource(_message.Message):
    __slots__ = ("success", "item")
    SUCCESS_FIELD_NUMBER: _ClassVar[int]
    ITEM_FIELD_NUMBER: _ClassVar[int]
    success: bool
    item: bytes
    def __init__(self, success: bool = ..., item: _Optional[bytes] = ...) -> None: ...

class Packet(_message.Message):
    __slots__ = ("type", "value")
    TYPE_FIELD_NUMBER: _ClassVar[int]
    VALUE_FIELD_NUMBER: _ClassVar[int]
    type: PacketId
    value: bytes
    def __init__(self, type: _Optional[_Union[PacketId, str]] = ..., value: _Optional[bytes] = ...) -> None: ...
