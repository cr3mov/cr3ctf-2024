from z3 import *


s = Solver()
keys = [
0xac,0x23,0xef,0x8e,0x92,0xd8,0xef,0xc5,0x4f,0x99,0x24,0xbc,0xc2,0xf9,0xbf,0xf3,0x37,0xb4,0xfb,0x3d,0x6c,0x1b,0x16,0x36,0x5,0x30,0x9f,0xd6,0x56,0x24,0x16,0x7,0xda,0x3b,0x48,0x1c,0xf7,0xb1
]
encrypted = [
0xa9,0x37,0xba,0x93,0x97,0x8e,0xe7,0xc4,0x5b,0xcb,0x36,0xef,0xfb,0xaf,0xb7,0xca,0x37,0xe3,0xf3,0x3f,0x3b,0x13,0x17,0xf,0x17,0x3e,0xca,0xef,0x56,0x23,0x1b,0x4,0xe3,0x3b,0x42,0x1b,0xf6,0xaa
]
FLAG = [BitVec(f'FLAG_{i}', 8) for i in range(len(keys))]


for i in range(len(FLAG)):
    s.add((FLAG[i] ^ 0x66) >= 33)
    s.add((FLAG[i] ^ 0x66) <= 127)


for i in range(len(keys)):
    s.add(FLAG[i] ^ keys[i] == encrypted[i])


s.check()
model = s.model()

for x in FLAG:
    print(chr(model[x].as_long() ^ 0x66), end='')
print()
