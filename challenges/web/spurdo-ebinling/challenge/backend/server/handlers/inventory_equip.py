from ..const.items import ItemInfo, ensure_item
from ..core.handlers import MessageHandler
from ..core.session import Session
from ..enums.packet_type import EPacketType
from ..proto.protocol_pb2 import CInventoryEquip, SInventoryEquip


handler = MessageHandler(EPacketType.INVENTORY_EQUIP)
handler.set_reject_value(SInventoryEquip(success=False))


@handler.validator()
async def on_inventory_equip_validator(session: Session, message: CInventoryEquip) -> bool:
    if message.item.name in [x.name for x in session.game.inventory]:
        return False

    return True


@handler.dispatcher()
async def on_inventory_equip(session: Session, message: CInventoryEquip) -> SInventoryEquip:
    item_info = ensure_item(message.item)
    if item_info is None:
        session.log_warn(f'Attempted to equip an unknown item {message.item.name}')
        return handler.reject_value  # type: ignore

    session.game.inventory.append(ItemInfo(
        name=item_info.name,
        damage=item_info.damage,
        icon=item_info.icon,
        chance=100
    ))
    session.log_info(f'Equipped {item_info.name}')

    return SInventoryEquip(success=True)
