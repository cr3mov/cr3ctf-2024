from pathlib import Path

import cv2
from pyzbar.pyzbar import decode


def le(value: bytes, signed: bool = False) -> int:
    """
    Load little endian number from bytes or string(hexdump)
    :param value: Bytes or string to load
    :param signed: Whether the value is signed

    :return: parsed integer
    :rtype: int
    """
    return int.from_bytes(value, 'little', signed=signed)


def to_le(value: int, size: int = 4, signed: bool = False) -> bytes:
    """
    Get little endian number as bytes
    :param value: Integer to dump
    :param size: Size in bytes
    :param signed: Whether the value is signed

    :return: dumped bytes
    :rtype: bytes
    """
    return value.to_bytes(size, 'little', signed=signed)


def chunks(line, n: int) -> list:
    return [line[i:i+n] for i in range(0, len(line), n)]


def decrypt_u8(val: int) -> int:
    vv = chunks(bin(val)[2:].rjust(8, '0'), 4)
    return int(vv[0], 2) ^ int(vv[1], 2)


def decrypt_u32(lhs, rhs) -> int:
    lhs = le(bytes(lhs))
    rhs = le(bytes(rhs))
    return lhs ^ rhs


data = []

rom_p = Path(__file__).parent / 'rom'
for file in list(rom_p.iterdir()):
    detected = decode(cv2.cvtColor(cv2.imread(str(file.absolute()), ), cv2.COLOR_BGR2RGB))
    assert len(detected) == 1

    decoded_bytes = [int(x, 2) for x in chunks(detected[0].data.decode(), 8)]

    opcode = decrypt_u8(decoded_bytes[0])
    op1 = decrypt_u8(decoded_bytes[1])
    op2 = decrypt_u8(decoded_bytes[2])
    imm = decrypt_u32(decoded_bytes[3:7], decoded_bytes[7:])

    data.append({
        'opcode': opcode,
        'op1': op1,
        'op2': op2,
        'imm': imm
    })

for insn in data:
    opcode = insn['opcode']
    op1 = insn['op1']
    op2 = insn['op2']
    imm = insn['imm']

    reverse = imm > 0

    if opcode == 9:
        print(f'data_t(UP, {str(reverse).lower()}),')
    elif opcode == 10:
        print(f'data_t(DOWN, {str(reverse).lower()}),')
    elif opcode == 11:
        print(f'data_t(LEFT, {str(reverse).lower()}),')
    elif opcode == 12:
        print(f'data_t(RIGHT, {str(reverse).lower()}),')
    elif opcode == 13:
        print(f'data_t(FRONT, {str(reverse).lower()}),')
    elif opcode == 14:
        print(f'data_t(BACK, {str(reverse).lower()}),')

