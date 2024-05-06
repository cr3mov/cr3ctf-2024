from dataclasses import dataclass
from typing import Literal

from ..enums.packet_type import EPacketType
from .session import Session


NET_ENDIANNESS: Literal['little', 'big'] = 'little'
V1_MAGIC: int = int.from_bytes(b'meme', byteorder=NET_ENDIANNESS)
HEADER_SIZE: int = 6


@dataclass(frozen=True)
class Message:
    magic: int
    packet_type: EPacketType
    packet_data: bytearray


def read_message(session: Session, raw_data: bytes) -> Message:
    if len(raw_data) < HEADER_SIZE:
        session.log_critical(f'Got only {len(raw_data)} bytes of data! Minimal size is {HEADER_SIZE}')

    magic: int = int.from_bytes(raw_data[0:4], byteorder=NET_ENDIANNESS)
    packet_type: int = int.from_bytes(raw_data[4:6], byteorder=NET_ENDIANNESS)

    if magic != V1_MAGIC:
        session.log_critical('Invalid magic')

    try:
        e_packet_type: EPacketType = EPacketType(packet_type)
    except ValueError:
        session.log_critical(f'Invalid packet id: {packet_type}')

    return Message(
        magic=magic,
        packet_type=e_packet_type,
        packet_data=bytearray(raw_data[HEADER_SIZE:])
    )


def assemble_message_with_header(packet_type: EPacketType, response: bytes) -> bytearray:
    result = bytearray()
    result.extend(V1_MAGIC.to_bytes(4, byteorder=NET_ENDIANNESS))
    result.extend(packet_type.value.to_bytes(2, byteorder=NET_ENDIANNESS))
    result.extend(response)
    return result
