from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from Crypto.Util.number import GCD, long_to_bytes
import json, itertools, math
from hashlib import sha256

with open('output.txt') as f:
    N = int(f.readline().split(' = ')[1])
    out = eval(f.readline().split(' = ')[1])

iv = bytes.fromhex(out['iv'])
ct = bytes.fromhex(out['ct'])

for G in range(2**9):
    g = N**G - 1
    key = sha256(str(g).encode()).digest()[:16]
    cipher = AES.new(key, AES.MODE_CBC, iv)
    m = cipher.decrypt(ct)
    if b'cr3{' in m:
        print(unpad(m, 16).decode())
        break