from numpy import uint8
from z3 import *
from z3smt import Breaker, MT19937
from time import time

with open('./flag.enc.bin', 'rb') as f:
    encrypted = f.read()

st_0 = time()
seed_1 = int.from_bytes(encrypted[0:8], 'little')
seed_2 = int.from_bytes(encrypted[8:12], 'little')
seed_3 = int.from_bytes(encrypted[12:16], 'little')

print(f'[+] seed_1: {hex(seed_1)}')
print(f'[+] seed_2: {hex(seed_2)}')
print(f'[+] seed_3: {hex(seed_3)}')

encrypted = bytearray(encrypted[16:])

br = Breaker()
s = Solver()

output_some_1 = BitVec('output_some_1', 32)
output_some_2 = BitVec('output_some_2', 32)
output_some_3 = BitVec('output_some_3', 32)

xxt = output_some_1 + (len(encrypted) // 7)
xxd = output_some_2 ^ output_some_3

s.add(xxt == seed_2)
s.add(xxd == seed_3)

print('[+] recovering seed')
recovered_seed = br.get_seed_mt(s, [
    (0, output_some_1),
    (1, output_some_2),
    (2, output_some_3),
])
assert recovered_seed is not None
# recovered_seed = 0x23a9c80b

print('[+] seed is:', hex(recovered_seed))

mt = MT19937(c_seed=recovered_seed)
[mt.extract_number() for _ in range(3)]

vvv = (mt.extract_number() << 32) | mt.extract_number()
orig_seed = seed_1 ^ vvv
print('[+] original seed:', hex(orig_seed))

keys = [
    mt.extract_number(),
    mt.extract_number(),
    mt.extract_number(),
    mt.extract_number(),
]
print('[+] keys:', ';'.join(map(hex, keys)))

delta = mt.extract_number()
print('[+] delta:', hex(delta))

num_rounds = min(max(delta & 0xFF, 32), 64)
print('[+] num_rounds:', hex(num_rounds))


def rotr8(value, shift):
    shift %= 8
    return ((value >> shift) | (value << (8 - shift))) & 0xFF


def rotl8(value, shift):
    shift %= 8
    return ((value << shift) | (value >> (8 - shift))) & 0xFF


st = time()
print('[+] reverting pt.1')
operations = []
for i in range(orig_seed % 1337):
    v1 = (mt.extract_number() & 0xF) * (mt.extract_number() & 0b111)
    pp = v1 % ((mt.extract_number() & 0xF) + 5)
    pp ^= uint8(orig_seed & 0b111)
    pp &= 0xFF
    for k in range(pp):
        for j in range(len(encrypted)):
            n = (mt.extract_number() & 0xFF) % 6

            imm = 0
            if n != 3 and n != 4 and n != 5:
                imm = mt.extract_number()

            operations.append([n, j, imm])

print(f'[+] {len(operations)} operations to revert')
for (n, j, imm) in reversed(operations):
    if n == 0:
        encrypted[j] = encrypted[j] ^ (imm & 0xFF)
    elif n == 1:
        encrypted[j] = (encrypted[j] - (imm & 0xFF)) & 0xFF
    elif n == 2:
        encrypted[j] = (encrypted[j] + (imm & 0xFF)) & 0xFF
    elif n == 3:
        encrypted[j] = rotr8(encrypted[j], 1)
    elif n == 4:
        encrypted[j] = rotl8(encrypted[j], 1)
    elif n == 5:
        encrypted[j] = (~uint8(encrypted[j])) & 0xFF
    else:
        raise ValueError()

print(f'[+] took {time() - st}')


def xtea_decrypt_int(k, v, delta_v=0x9e3779b9, rounds=32):
    v0, v1 = v

    delta, mask = delta_v, 0xffffffff
    sum = (delta * rounds) & mask
    for _ in range(rounds):
        v1 = (v1 - (((v0 << 4 ^ v0 >> 5) + v0) ^
                    (sum + k[sum >> 11 & 3]))) & mask
        sum = (sum - delta) & mask
        v0 = (v0 - (((v1 << 4 ^ v1 >> 5) + v1) ^
                    (sum + k[sum & 3]))) & mask

    return v0, v1


print('[+] reverting pt.2')
flag = []

for i in range(len(encrypted) // 8):
    raw = encrypted[i * 8:(i + 1) * 8]

    vals = [
        int.from_bytes(raw[:4], 'little'),
        int.from_bytes(raw[4:], 'little'),
    ]

    flag = [*flag, *xtea_decrypt_int(keys, vals, delta, num_rounds)]

flag_str = ''
for x in flag:
    flag_str += x.to_bytes(4, 'little').decode()

print('[+] flag:', flag_str)
print('[+] total execution time:', time() - st_0)
