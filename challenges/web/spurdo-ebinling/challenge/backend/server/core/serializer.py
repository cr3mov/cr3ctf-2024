from google.protobuf.message import Message as ProtoMessage
from loguru import logger

from ..proto.protocol_pb2 import (
    CInventory,
    CInventoryEquip,
    CLevelUp,
    CPing,
    CRequestResource,
    Packet,
    PacketId,
    SAchievementUnlocked,
    SInventory,
    SInventoryEquip,
    SLevelUp,
    SPing,
    SRequestResource,
    Wave,
)


@logger.catch
def serialize_message(item: ProtoMessage) -> bytes:
    out_packet = Packet()

    if isinstance(item, SAchievementUnlocked):
        out_packet.type = PacketId.PACKET_ID_ACHIEVEMENT_UNLOCKED
    elif isinstance(item, SInventory) or isinstance(item, CInventory):
        out_packet.type = PacketId.PACKET_ID_INVENTORY
    elif isinstance(item, SInventoryEquip) or isinstance(item, CInventoryEquip):
        out_packet.type = PacketId.PACKET_ID_INVENTORY_EQUIP
    elif isinstance(item, Wave):
        out_packet.type = PacketId.PACKET_ID_WAVE
    elif isinstance(item, SLevelUp) or isinstance(item, CLevelUp):
        out_packet.type = PacketId.PACKET_ID_LEVEL_UP
    elif isinstance(item, SPing) or isinstance(item, CPing):
        out_packet.type = PacketId.PACKET_ID_PING
    elif isinstance(item, SRequestResource) or isinstance(item, CRequestResource):
        out_packet.type = PacketId.PACKET_ID_REQUEST_RESOURCE
    else:
        out_packet.type = PacketId.PACKET_ID_PING

    out_packet.value = item.SerializeToString()

    return out_packet.SerializeToString()
