from z3 import *
from typing import List


s = Solver()
N = 2
flag = b'cr3{w4it_4r_1t!}'
imm = round(0x6372337B5730777D / 2) * -1

assert len(flag) <= 8 * N


def chunks(line: bytes, n: int) -> List[bytes]:
    return [line[i:i+n] for i in range(0, len(line), n)]


flag_segments = [int.from_bytes(x, 'little') for x in chunks(flag, 8)]

rotlc = [BitVec(f'rotlc{i}', 64) for i in range(N)]
xorc = [BitVec(f'xorc{i}', 64) for i in range(N)]
shrc = [BitVec(f'shrc{i}', 64) for i in range(N)]
xorc2 = [BitVec(f'xorc2_{i}', 64) for i in range(N)]

for i in range(len(rotlc)):
    s.add(rotlc[i] != 0)
    s.add(xorc[i] != 0)
    # s.add(shrc[i] != 0)
    s.add(xorc2[i] != 0)

imms = [BitVec(f'imm_{i}', 64) for i in range(N)]
exprs = [0 for i in range(N)]


for i in range(len(imms)):
    s.add(imms[i] == imm)

    exprs[i] = imms[i]
    exprs[i] = RotateLeft(exprs[i], rotlc[i])
    exprs[i] ^= xorc[i]
    exprs[i] = LShR(exprs[i], shrc[i])
    exprs[i] ^= xorc2[i]

for i in range(len(imms)):
    s.add(exprs[i] == flag_segments[i])

assert s.check() == sat
m = s.model()

for i in range(len(exprs)):
    print(f'values[{i}] = static_cast<NumTy>(std::rotl(static_cast<std::uint64_t>(values[{i}]), static_cast<NumTy>({hex(m[rotlc[i]].as_long())})));')
    print(f'values[{i}] ^= static_cast<NumTy>({hex(m[xorc[i]].as_long())});')
    print(f'values[{i}] >>= {hex(m[shrc[i]].as_long())};')
    print(f'values[{i}] ^= static_cast<NumTy>({hex(m[xorc2[i]].as_long())});')
    print()

[s.add(x == m[x].as_long()) for x in rotlc]
[s.add(x == m[x].as_long()) for x in xorc]
[s.add(x == m[x].as_long()) for x in shrc]
[s.add(x == m[x].as_long()) for x in xorc2]

dummy_shit = [BitVec(f'dummy_{i}', 64) for i in range(N)]
[s.add(x != imm) for x in dummy_shit]

for i in range(len(imms)):
    s.add(substitute(exprs[i], (imms[i],dummy_shit[i])) == flag_segments[i])

assert s.check() == unsat
