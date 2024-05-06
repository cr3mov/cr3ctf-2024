from typing import Type

from google.protobuf.message import Message as ProtoMessage

from ..proto.protocol_pb2 import CInventory, CInventoryEquip, CLevelUp, CPing, CRequestResource, Packet, PacketId, Wave
from .protocol import Message
from .session import Session


PACKETS = {
    PacketId.PACKET_ID_PING: CPing,
    PacketId.PACKET_ID_WAVE: Wave,
    PacketId.PACKET_ID_INVENTORY: CInventory,
    PacketId.PACKET_ID_INVENTORY_EQUIP: CInventoryEquip,
    PacketId.PACKET_ID_LEVEL_UP: CLevelUp,
    PacketId.PACKET_ID_REQUEST_RESOURCE: CRequestResource,
}


def load_message(session: Session, message: Message) -> ProtoMessage:
    c_packet = Packet()
    try:
        c_packet.ParseFromString(bytes(message.packet_data))
    except IOError:
        session.log_critical(f'Got invalid message: {message.packet_data}')

    if c_packet.type not in PACKETS.keys():
        session.log_critical(f'Unknown message to handle: {message.packet_type}')

    cls_type: Type[ProtoMessage] = PACKETS[c_packet.type]
    result: ProtoMessage = cls_type()

    try:
        result.ParseFromString(c_packet.value)
    except IOError:
        session.log_critical(f'Got invalid message(2): {c_packet.value!r} ({c_packet.type})')

    return result
