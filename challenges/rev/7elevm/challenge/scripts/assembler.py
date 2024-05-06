from pprint import pprint

from treepoem import generate_barcode
from os import remove
from enum import IntEnum
from numpy import uint8, uint32, uint64
from pathlib import Path
from random import seed, randint, choice
from typing import List
from multiprocessing import Pool


seed(2033474659)
# 1Il|Il_il_i_ll11Iiii_|_l1I_l1i|1|_iI|_||i1IIl|I1


class ErrorNotImplemented(Exception):
    pass


def urandom(n: int) -> bytes:
    r = bytearray()
    for i in range(n):
        r += bytes([randint(0x00, 0xFF)])
    return bytes(r)


def le(value: bytes, signed: bool = False) -> int:
    return int.from_bytes(value, 'little', signed=signed)


def to_le(value: int, size: int = 4, signed: bool = False) -> bytes:
    return value.to_bytes(size, 'little', signed=signed)


def chunks(line: bytes, n: int) -> List[bytes]:
    return [line[i:i+n] for i in range(0, len(line), n)]


def serialize_barcode(pp) -> None:
    payload, out_file = pp
    payload_str = ''
    for b in payload:
        payload_str += bin(int(b))[2:].rjust(8, '0')

    generate_barcode(
        barcode_type='code39ext',
        data=payload_str,
    ).convert('1').save(out_file)


def enc_byte(val):
    key = int(le(urandom(1))) & 0b1111
    val = int(uint8(val) ^ uint8(key))
    br = bin(key)[2:].rjust(4, '0')
    br += bin(val)[2:].rjust(4, '0')
    return int(br, base=2)


def enc_u32(val):
    key = int(le(urandom(4)))
    val = int(uint32(val) ^ uint32(key))
    br = bin(key)[2:].rjust(32, '0')
    br += bin(val)[2:].rjust(32, '0')
    return int(br, base=2)


commands = []


def serialize_insn(opcode, op1: int = 0, op2: int = 0, imm: int = 2 ** 31):
    buf = to_le(enc_byte(opcode), 1)
    buf += to_le(enc_byte(op1), 1)
    buf += to_le(enc_byte(op2), 1)
    buf += to_le(enc_u32(imm), 8)
    commands.append(buf)
    return buf


add_insn = serialize_insn


class CMD(IntEnum):
    MOV_IMM = 0
    ADD = 1
    CMP = 2
    JZ = 3
    XOR = 4
    SUB = 5
    PUSH = 6
    INPUT_OUTPUT = 7
    EXIT = 8
    CUBE_UP = 9
    CUBE_DOWN = 10
    CUBE_LEFT = 11
    CUBE_RIGHT = 12
    CUBE_FRONT = 13
    CUBE_BACK = 14
    CUBE_CMP_SIDES = 15


def rand_op() -> int:
    return le(urandom(1)) % 6


def push_imm(imm: int):
    serialize_insn(CMD.PUSH, rand_op(), rand_op(), imm=imm)


def print_msg(message: str):
    push_imm(0)  # nullterm
    for chunk in reversed(chunks(message.encode(), 4)):
        push_imm(le(chunk))
    serialize_insn(CMD.INPUT_OUTPUT, rand_op(), rand_op(), 1)  # output


[print_msg(x) for x in '''⠀⠀⢀⡀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⠀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⢠⢿⣳⠋⣿⢋⡇⠀⠀⠸⡈⡼⣿⠢⡈⢦⡀⠀⢱⢤⠀⠀⠀⠀⠮⠊⠱⠂
⠀⡞⢡⡏⣾⠸⠎⢀⡆⡼⣄⠑⢱⣿⡖⠈⢪⣷⡄⠀⣷⢣⠀⠀⠀⠀⡠⡬⠂
⠰⢀⣾⣴⠛⡀⠀⢸⣧⡏⢻⣷⢸⣿⡧⣰⣶⠿⣹⡄⢉⠻⣇⠀⠀⣀⡋⠀⠀
⠀⠜⡱⠏⠀⡇⢠⣾⢹⣁⣨⡽⣿⣿⡀⠉⣿⡶⠉⢻⣿⡄⡿⡄⠀⠙⠃⠀⠀
⠀⢸⡀⣦⢴⣷⣼⣿⠸⣿⣿⣧⠈⠃⠀⠀⠀⠀⠀⠀⣿⣇⡇⠀⠀⠀⠀⠀⠀
⢸⣿⣿⣿⣾⣿⣿⣷⠈⠂⠙⠊⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⡇⠀⠀⠀⠀⠀⠀
⢸⣿⣿⣿⣿⣿⣿⣿⣷⣆⠀⠀⠀⠀⠀⠀⠀⠀⣠⢿⣿⡏⠀⠀⠀⠀⠀⠀⠀
⠆⣿⣿⣿⣿⣿⣿⣿⣿⠙⢷⣦⣤⡄⠀⠀⠀⠈⢼⠸⡿⠀⠀⠀⠀⠀⠀⠀⠀
⠀⢿⢹⣿⣿⣿⣿⣿⣿⡇⠈⠉⠻⠟⠄⠀⠄⠀⠀⠠⠇⠀⠀⠀⠀⠀⠀'''.splitlines()]

# print_msg("We have just received a new type of Rubik's cube for sale.")
# print_msg("However, we can't sell it unless it has been solved at least once.")
# print_msg("Can you please test it out using your flag?")
serialize_insn(CMD.INPUT_OUTPUT, rand_op(), rand_op(), 0)  # input

for i in range(100):
    serialize_insn(choice([
        CMD.CUBE_UP,
        CMD.CUBE_DOWN,
        CMD.CUBE_LEFT,
        CMD.CUBE_RIGHT,
        CMD.CUBE_FRONT,
        CMD.CUBE_BACK
    ]), rand_op(), rand_op(), imm=choice([0, 1]))

imm_operations = [[]] * 6

for i in range(len(imm_operations)):
    for _ in range(randint(3, 7)):
        imm = uint64(randint(0x10101010, 0x13333337))

        it = choice([
            [
                lambda x, imm: x + imm,
                CMD.SUB,
            ],
            [
                lambda x, imm: x ^ imm,
                CMD.XOR,
            ],
            [
                lambda x, imm: x - imm,
                CMD.ADD,
            ]
        ])

        imm_operations[i].append({
            'apply': it[0],
            'revert': it[1],
            'imm': imm
        })


enc_registers = [
    0x00000000,
    0x11111111,
    0x22222222,
    0x33333333,
    0x44444444,
    0x55555555,
]

for i in range(len(imm_operations)):
    for operation in imm_operations[i]:
        enc_registers[i] = int(operation['apply'](uint64(enc_registers[i]), operation['imm']))
        print(operation['revert'], hex(operation['imm']), hex(int(enc_registers[i])))


print('enc_registers:')
pprint([hex(x) for x in enc_registers])

for i in range(len(imm_operations)):
    serialize_insn(CMD.MOV_IMM, i, rand_op(), enc_registers[i])

for i in range(len(imm_operations)):
    for op in reversed(imm_operations[i]):
        serialize_insn(op['revert'], i, rand_op(), op['imm'])

serialize_insn(CMD.CUBE_CMP_SIDES, rand_op(), rand_op())
serialize_insn(CMD.JZ, rand_op(), rand_op(), imm=5)

# if matches
print_msg('Correct')
add_insn(CMD.EXIT)

# if not
print_msg('Incorrect')
add_insn(CMD.EXIT)

# ***

out_dir = Path(__file__).parent / 'rom'
if out_dir.exists():
    [remove(x) for x in out_dir.iterdir()]
    out_dir.rmdir()
out_dir.mkdir()

print(f'got {len(commands)} commands')

with Pool(12) as p:
    args = []
    for i, payload in enumerate(commands):
        args.append((payload, out_dir / f'{str(i).rjust(4, "0")}.png',))
    p.map(serialize_barcode, args)
