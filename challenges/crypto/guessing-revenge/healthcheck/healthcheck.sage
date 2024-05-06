#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from os import environ

environ['TERM'] = 'linux'
environ['TERMINFO'] = '/etc/terminfo'

from pwn import remote, process, context # noqa
from sage.all import * # noqa


def handle_pow(r):
    r.recvline()
    challenge = r.recvline().decode('ascii').strip()
    print(f'Solving pow -> {challenge}')
    p = process(['bash', '-c', challenge])
    solution = p.recvline().strip()
    p.close()
    r.recvuntil(b'solution: ')
    r.sendline(solution)


with context.quiet:
    io = remote('challenge', 1337)
tmp_data = io.recv(len('proof'), timeout=2)
if tmp_data == b'proof':
    handle_pow(io)
else:
    io.unrecv(tmp_data)

io.recvline()


def serialize_mtx(mtx):
    rows = []
    for row in mtx.rows():
        rows.append(','.join(str(elem) for elem in row))
    return '[' + ';'.join(rows) + ']'


def deserialize_mtx(mtx):
    rows = mtx.strip('[]\n').split(';')
    return Matrix(F, matrix([[int(num) for num in row.split(',')] for row in rows]))


def submit_b(mtx):
    io.sendline(serialize_mtx(mtx).encode())
    r = io.recvline()
    if r.startswith(b'https:'):
        raise ValueError(r)
    return deserialize_mtx(r.decode())


p = (2 ** 31) - 1
F = GF(p)
FLAG_N = 55


def create_diagonal_matrices(n, a):
    matrices = []
    for i in range(n):
        matrix = Matrix(F, identity_matrix(n))
        matrix[i, i] = a
        matrices.append(matrix)
    return matrices


def process_matrices(matrices):
    results = []
    for matrix in matrices:
        response = submit_b(matrix)
        if isinstance(response, str):
            raise ValueError('dammit:' + str(response))
        else:
            inv_matrix = matrix.inverse()
            x_i = inv_matrix * response
            results.append(x_i)
    return results


def sum_and_normalize(results, a):
    sum_vectors = sum(results)
    n = len(results)
    normalized_result = sum_vectors / F(n - 1 + 1/a)
    return normalized_result


a = F(3)
matrices = create_diagonal_matrices(FLAG_N, a)

results = process_matrices(matrices)

with context.quiet:
    io.close()

out_flag_mtx = sum_and_normalize(results, a)

out_flag = ''
for x in out_flag_mtx:
    for y in x:
        out_flag += chr(y)

print(out_flag, end=', ', flush=False)
open('/out', 'w').write(str(0 if 'cr3{' in out_flag else 1))
