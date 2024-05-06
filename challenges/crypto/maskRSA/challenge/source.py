from Crypto.Util.number import getPrime, bytes_to_long
import random

p = getPrime(1024)
q = getPrime(1024)
n = p * q

flag = b'cr3{f4ke_fl4g}'
assert len(flag) == 28
flag1 = bytes_to_long(flag[:14])
flag2 = bytes_to_long(flag[14:])

masks = [random.getrandbits(n.bit_length()) for i in range(20)]
enc = [pow(mask * flag1 + (mask + 1) * flag2, 17, n) for mask in masks]

print(f'n = {n}')
print(f'masks = {masks}')
print(f'enc = {enc}')
