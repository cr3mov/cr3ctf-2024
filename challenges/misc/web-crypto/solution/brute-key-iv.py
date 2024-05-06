import base64

from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

DATA = base64.b64decode("3X6pTMUO9K/YCbuzxE5a0LSegv/+/LtKdskX/dr0F844hyk/xGELxH0I/JY1zlOCO7MBzeeFEtTKRTWmVStDh8umar2er+BFRK9jvgvhYfCMC1Ri4RBzhL8zLhdZDCwS1Yak1oHW/x7j/a7fTLavWvUNck1oHZgmFKq91UtXom/ziGxqrqls+G/KRJ4CiHftGBrN/PYrHCCludcgqK+QZw==")

for c in range(256):
  for c1 in range(256):
    cipher = AES.new(c.to_bytes(1, 'little') * 32, AES.MODE_CBC, iv=c1.to_bytes(1, 'little') * 16)
    d = cipher.decrypt(DATA)
    try:
      data = unpad(d, 16)
      if data.startswith(b"cr3"):
        print(data)
    except Exception as e:
      pass