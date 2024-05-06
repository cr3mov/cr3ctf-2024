#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from os import environ

environ['TERM'] = 'linux'
environ['TERMINFO'] = '/etc/terminfo'

from sage.all import *  # noqa
from aes import *  # noqa
from pwn import remote, process, context, xor  # noqa
from Crypto.Cipher import AES  # noqa
from Crypto.Util.number import long_to_bytes  # noqa


def handle_pow(r):
    r.recvline()
    challenge = r.recvline().decode('ascii').strip()
    print(f'Solving pow -> {challenge}')
    p = process(['bash', '-c', challenge])
    solution = p.recvline().strip()
    p.close()
    r.recvuntil(b'solution: ')
    r.sendline(solution)


success = False


# retry a couple of times
for i in range(10):

    with context.quiet:
        conn = remote('challenge', 1337)
    tmp_data = conn.recv(len('proof'), timeout=2)
    if tmp_data == b'proof':
        handle_pow(conn)
    else:
        conn.unrecv(tmp_data)
    conn.recvline()

    hist = []
    for i in range(9):
        payload = b''.join([bytes([0] * j + [i * 2 + 1] + [0] * (3 - j)) for j in range(4)]).hex().encode()
        conn.sendline(b'1')
        conn.sendline(b'00')
        conn.sendline(payload)

        out = conn.recvlines(5)
        ciphertext = int(out[-1].strip().split(b'matrix(hex): ')[1])
        ciphertext = long_to_bytes(ciphertext, 16)
        hist.append(ciphertext)

    # recover the last round key
    round_key = []
    for index in range(16):
        # print('KEY', index)

        arr = [[0 for _ in range(256)] for _ in range(256)]

        for i in range(9):
            for x in range(256):
                for k in range(256):
                    if Sbox[x * (2 * i + 1) % 256] ^ k == hist[i][index]:
                        arr[x][k] += 1
                        if arr[x][k] == 9:
                            # print(k)
                            round_key.append(k)
    # print('round key:', round_key)
    round_key = bytes(round_key)


    # recover initial key
    # from https://github.com/fanosta/aeskeyschedule/blob/master/aeskeyschedule/aeskeyschedule.py
    def rot_word(word):
        return bytes((word[(i + 1) % 4] for i in range(4)))


    def sub_word(word: bytes) -> bytes:
        return bytes((Sbox[w] for w in word))


    rcon = [x.to_bytes(4, 'little') for x in [0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36, ]]

    for i in range(9, -1, -1):
        a2 = round_key[0:4]
        b2 = round_key[4:8]
        c2 = round_key[8:12]
        d2 = round_key[12:16]

        d1 = xor(d2, c2)
        c1 = xor(c2, b2)
        b1 = xor(b2, a2)
        a1 = xor(a2, rot_word(sub_word(d1)), rcon[i])

        round_key = a1 + b1 + c1 + d1

    # print('key:', round_key.hex())
    # decrypt flag, you can write the decryption function for the custom AES or just send identity matrix to get the
    # normal AES encryption of the flag
    payload = b''.join([bytes([0] * j + [1] + [0] * (3 - j)) for j in range(4)]).hex().encode()
    conn.sendline(b'2')
    conn.sendline(payload)
    out = conn.recvlines(5)
    flag = int(out[-1].strip().split(b'matrix(hex): ')[1])
    cipher = AES.new(round_key, AES.MODE_ECB)

    with context.quiet:
        conn.close()

    out_flag = (cipher.decrypt(long_to_bytes(flag, 16)).rstrip(b'}') + b'}').decode('utf-8', 'ignore')
    print(out_flag)

    if 'cr3{' in out_flag:
        success = True
        break

open('/out', 'w').write(str(0 if success else 1))
