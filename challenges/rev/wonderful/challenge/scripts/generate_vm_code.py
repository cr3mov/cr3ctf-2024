from random import choice, seed, randint
from random import shuffle

from numpy import uint64

seed(0x3c7007fe7d25431a)
flag = 'cr3{0hw0www_YoU_4re_Th3_real_d34L!Ih0p3_y0u_werent_sc4red!!!!!!_hhmmpfrdkfegtsniczjyew6s3jiugm7mdev}'


def dump_as_c(data: bytearray) -> str:
    out = ''
    for i, x in enumerate(data):
        if i > 0:
            out += ', '
        out += f'{hex(x)}'
    return out


def build_stub(num: int, expected_ch: str):
    flags = ''
    result = bytearray()

    # load char
    # mov cl, byte ptr [rcx]
    result += b'\x8A\x09'
    # and rcx, 0xFF
    result += b'\x48\x81\xE1\xFF\x00\x00\x00'

    # Init rdx
    x = randint(0, 2)
    generated_imm = 0
    if x == 0:
        flags += 'imm64;'
        generated_imm = randint(1, 0xFF)
        # mov rdx, imm64
        result += b'\x48\xBA' + generated_imm.to_bytes(8, 'little')
    elif x == 1:
        flags += 'peb_check;'
        generated_imm = (0 + 1) << 4
        # mov r10, gs:0x60
        result += b'\x65\x4C\x8B\x14\x25\x60\x00\x00\x00'
        # xor rdx, rdx
        result += b'\x48\x31\xD2'
        # mov dl, byte ptr [r10 + 0x2]
        result += b'\x41\x8A\x52\x02'
        # add edx, 1
        result += b'\x83\xC2\x01'
        # shl rdx, 4
        result += b'\x48\xC1\xE2\x04'
    elif x == 2:
        flags += 'kusd_check;'
        generated_imm = (0 + 1) << 4
        # xor rdx, rdx
        result += b'\x48\x31\xD2'
        # mov dl, byte ptr [0x7ffe02d4]
        result += b'\x8A\x14\x25\xD4\x02\xFE\x7F'
        # add edx, 1
        result += b'\x83\xC2\x01'
        # shl rdx, 4
        result += b'\x48\xC1\xE2\x04'
    else:
        raise ValueError()

    # mov rax, rcx
    result += b'\x48\x89\xC8'

    x = randint(0, 2)
    expected_res = 0
    if x == 0:
        flags += 'add;'
        # add rax, rdx
        result += b'\x48\x01\xD0'
        expected_res = uint64(uint64(ord(expected_ch)) + generated_imm)
    elif x == 1:
        flags += 'sub;'
        # sub rax, rdx
        result += b'\x48\x29\xD0'
        expected_res = uint64(uint64(ord(expected_ch)) - generated_imm)
    elif x == 2:
        flags += 'xor;'
        # xor rax, rdx
        result += b'\x48\x31\xD0'
        expected_res = uint64(uint64(ord(expected_ch)) ^ uint64(generated_imm))
    else:
        raise ValueError()

    flags += f'{hex(int(expected_res))}'

    # movabs r11, imm64
    result += b'\x49\xBB' + int(expected_res).to_bytes(8, 'little')
    # cmp rax, r11
    result += b'\x4C\x39\xD8'
    # setz al
    result += b'\x0F\x94\xC0'
    # and rax, 1
    result += b'\x48\x83\xE0\x01'

    # ret
    result += b'\xC3'

    ss = f'// {flags}\n'
    ss += f'volatile uint8_t data_{num}[{len(result)}] = ' + '{};\n'
    ss += '// clang-format off\n    '
    for i, v in enumerate(result):
        if i > 0:
            ss += ' '
        ss += f'data_{num}[{i}] = {hex(v)};'
    ss += '\n'
    ss += '// clang-format on\n'
    ss += f'res = ((uint64_t(*)(char*))data_{num})(&flag[{num}]);\n'
    ss += f'counter += res;\n'
    # ss += 'if (res > 0) __debugbreak();\n'
    ss += f'memset((void*)data_{num}, 0, {len(result)});'

    return ss


strs = [build_stub(i, c) for i, c in enumerate(flag)]
shuffle(strs)

print('\n\n'.join(strs))
