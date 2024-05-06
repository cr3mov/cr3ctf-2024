#!/home/sage/sage/src/bin/sage
from sage.all import *

p = (2 ** 31) - 1
F = GF(p)
FLAG_N = 55
try:
    FLAG_STR = open('/flag').read().strip()
except:  # noqa
    FLAG_STR = 'A' * FLAG_N
assert len(FLAG_STR) == FLAG_N

FLAG = matrix(F, FLAG_N, 1, [ord(x) for x in FLAG_STR])


def random_invertible_matrix(n):
    while True:
        res = matrix(F, n, n, [F.random_element() for _ in range(n * n)])
        if res.is_invertible():
            return res


def serialize_mtx(mtx):
    rows = []
    for row in mtx.rows():
        rows.append(','.join(str(elem) for elem in row))
    return '[' + ';'.join(rows) + ']'


def deserialize_mtx(mtx):
    rows = mtx.strip('[]\n').split(';')
    rows = [[int(num) for num in row.split(',')] for row in rows]
    return Matrix(F, rows)


A = random_invertible_matrix(FLAG_N)


def submit_b(B):
    if not B.is_invertible():
        return 'non invertible'

    n = B.nrows()
    if n != FLAG_N:
        return 'rows'

    diagonal_element = B[0, 0]

    identity_multiple = Matrix(F, identity_matrix(n))
    for i in range(n):
        identity_multiple[i, i] = diagonal_element

    if B == identity_multiple:
        return 'identity_multiple'

    return B * A * (B ^ -1) * (A ^ -1) * FLAG


print('aight, lets see what you got')
while True:
    try:
        matrix = deserialize_mtx(input())
        res = submit_b(matrix)
        if isinstance(res, str):
            print('https://youtu.be/G7b4VMEATNo?t=18')
            break
    except Exception as e:
        print('https://youtu.be/G7b4VMEATNo?t=19')
        break

    print(serialize_mtx(res))
