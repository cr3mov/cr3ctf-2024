from ..core.handlers import MessageHandler
from ..core.session import Session
from ..enums.packet_type import EPacketType
from ..proto.protocol_pb2 import CInventory, ItemDetail, SInventory


handler = MessageHandler(EPacketType.INVENTORY)


@handler.dispatcher()
async def on_inventory(session: Session, _: CInventory) -> SInventory:
    return SInventory(
        items=[
            SInventory.Item(
                equipped=False,
                detail=ItemDetail(
                    name=item.name,
                    damage=item.damage,
                    icon=item.icon,
                )
            ) for item in session.game.inventory
        ]
    )
