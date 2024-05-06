from dataclasses import dataclass
from typing import Callable

from loguru import logger

from ..const.chances import roll_chance
from ..proto.protocol_pb2 import ItemDetail


@dataclass(frozen=True)
class ItemInfo:
    name: str
    damage: int
    icon: str
    chance: int | float = 100


items = [
    # ItemInfo(name='ultra rare item 2', damage=1337, icon='rare', chance=0.01),
    # ItemInfo(name='ultra rare item 3', damage=1337, icon='rare', chance=0.01),
    # ItemInfo(name='ultra rare item 4', damage=1337, icon='rare', chance=0.01),
    # ItemInfo(name='ultra rare item 5', damage=1337, icon='rare', chance=0.01),
    ItemInfo(name='ultra rare item', damage=1337, icon='rare', chance=0.01),
    ItemInfo(name='default', damage=5, icon='default', chance=100),
]

ITEMS_COUNT: int = 6
DEFAULT_ITEM = items[-1]


@logger.catch
def _filter_item(
        filter_fn: Callable[[ItemInfo], bool] = lambda x: True,
        sort_fn: Callable[[ItemInfo], int] = lambda x: 0
) -> ItemInfo | None:
    for item in sorted(items, key=sort_fn):
        if filter_fn(item) is False:
            continue

        return item

    return None


@logger.catch
def random_item() -> ItemInfo:
    @logger.catch
    def filter_fn(item: ItemInfo) -> bool:
        return roll_chance(item.chance)

    @logger.catch
    def sort_fn(item: ItemInfo) -> int:
        return int(item.chance)

    result = _filter_item(
        filter_fn=filter_fn,
        sort_fn=sort_fn,
    )

    assert result is not None
    return result


@logger.catch
def ensure_item(item: ItemDetail) -> ItemInfo | None:
    @logger.catch
    def filter_fn(item_in: ItemInfo) -> bool:
        return item_in.name == item.name

    return _filter_item(
        filter_fn=filter_fn,
    )
