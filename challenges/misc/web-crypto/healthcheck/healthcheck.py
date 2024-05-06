#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import requests
import base64

from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

res = requests.post("http://challenge:1337/collection", data={"secret": "flag"}, cookies={
    "DEBUG": "camchito"
})
DATA = base64.b64decode(res.json()['data'])

for c in range(256):
    for c1 in range(256):
        cipher = AES.new(c.to_bytes(1, 'little') * 32, AES.MODE_CBC, iv=c1.to_bytes(1, 'little') * 16)
        d = cipher.decrypt(DATA)
        try:
            data = unpad(d, 16)
            if data.startswith(b"cr3"):
                print('flag', data)
                exit(0)
        except Exception as e:
            pass

exit(1)
