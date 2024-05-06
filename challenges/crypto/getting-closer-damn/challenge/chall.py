import os, json, random, math
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from Crypto.Util.number import getPrime, GCD
from hashlib import sha256
from secret import FLAG

def get_prod():
    return math.prod([random.choice(pool) for _ in range(3)])

FLAG = b'cr3{???????????????????????????????}'

N = getPrime(512)

pool = [getPrime(9) for _ in range(10)]
a, b = [get_prod() for _ in range(2)]

g = GCD(N**a - 1, N**b - 1) # pros of having a quantum computer ^-^

key = sha256(str(g).encode()).digest()[:16]
iv = os.urandom(16)
cipher = AES.new(key, AES.MODE_CBC, iv)
ct = cipher.encrypt(pad(FLAG, 16))
out = {'iv': iv.hex(), 'ct': ct.hex()}

with open('output.txt', 'w') as f:
    f.write(f'{N = }\n')
    f.write(f'{out = }')