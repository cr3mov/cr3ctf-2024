from random import randint
from random import seed

seed(16921077237318262398)

flag = b'cr3{d0nt_trU5t_D3c0mp1L3rs_t0o_muCh}'

for i in range(len(flag)):
    k = randint(5, 30)
    prev = f'flag[{i - 1}]' if i > 0 else '0'
    prev_val = flag[i - 1] if i > 0 else 0

    dkey1 = randint(5, 30)
    dkey2 = randint(5, 30)

    expected = (flag[i] - k) ^ prev_val
    print(
        f'DECOMP_BREAK_ALL;\nif (((flag[{i}] - decrypt_var<{hex(dkey1)}>({hex(k - dkey1 - 1)})) ^ {prev}) != decrypt_var<{hex(dkey2)}>({hex(expected - dkey2 - 1)}))',
        end=' {\n')
    print('\tvalid -= 1;\n} else {\n\tvalid += 1;\n}')

mkey = randint(5, len(flag) - 2)
print(f'\nconst char* message = valid == decrypt_var<{hex(mkey)}>({hex(len(flag) - mkey - 1)}) ? "that\'s right!\\n" : "nope\\n";')
print('printf(message);')
