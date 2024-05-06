from z3 import *


# Both `FLAG` and `FAKE_FLAG` should consist of the same number of chars
FLAG: str = 'cr3{t0ld_y0u_th4t_1t_w0uld_t4k3_15m1n}'
FAKE_FLAG = 'cr3{c0ngr4t5_0n_f1nd1ng_th3_fake_flag}'
ENC_COMM_KEY: int = 0x66
assert len(FLAG) == len(FAKE_FLAG)


s = Solver()
keys = [BitVec(f'keys_{i}', 8) for i in range(len(FLAG))]
keys2 = [BitVec(f'keys2_{i}', 8) for i in range(len(FLAG))]
fake_flag = [BitVec(f'fake_flag_{i}', 8) for i in range(len(FLAG))]
flag = [BitVec(f'flag_{i}', 8) for i in range(len(FLAG))]

for i in range(len(keys)):
    s.add(keys[i] != keys2[i])


for i in range(len(FLAG)):
    s.add(flag[i] == ord(FLAG[i]) ^ ENC_COMM_KEY)
    s.add(fake_flag[i] == ord(FAKE_FLAG[i]) ^ ENC_COMM_KEY)


def encrypt(name, src):
    result = [BitVec(f'{name}_{i}', 8) for i in range(len(src))]
    for i in range(len(src)):
        s.add(result[i] == src[i] ^ keys[i])
    return result


encrypted_fake_flag = encrypt('fake_flag_enc', fake_flag)


tampered_flag = [BitVec(f'tampered_flag_{i}', 8) for i in range(len(keys))]
for i in range(len(keys2)):
    s.add(tampered_flag[i] == RotateLeft(flag[i] ^ (keys2[i] + i) ^ i, 2))
encrypted_flag = encrypt('flag_enc', tampered_flag)


for i in range(len(FLAG)):
    s.add(encrypted_flag[i] == encrypted_fake_flag[i])


if s.check() != sat:
    print('unsat gg')
    exit()

print('sat')
model = s.model()


def dump(name, arr):
    print(f'{name} ', end='')
    for i in range(len(arr)):
        print(hex(model.eval(arr[i]).as_long()), end=',')
    print('')


print('kSize:', hex(len(FLAG)))
dump('keys', keys)
dump('keys2', keys2)
dump('encrypted_flag', encrypted_flag)
dump('encrypted_fake_flag', encrypted_fake_flag)
dump('tampered_flag', tampered_flag)
