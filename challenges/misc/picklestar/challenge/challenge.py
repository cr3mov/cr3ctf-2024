#!/usr/bin/env python3
import pickle
import pickletools

from blacklist import BLACKLISTED_OPCODES
from typing import List


p = bytes.fromhex(input('Gimme some pickles in hex: '))
if len(p) > 25:
    print("That's too much!")
    exit()


def get_ops() -> List[str]:
    try:
        for x in pickletools.genops(p):
            yield x[0].name
    except:
        pass


try:
    if any(p.count(x.encode()) > 0 for x in [
        'os',
        'system',
        'sys',
        'exec',
        'subprocess',
    ]):
        print('Are you trying to hack me? I thought we were friends!')
        exit()

    opcodes = list(get_ops())

    if len(opcodes) > 3:
        print("That's too much!")
        exit()

    if any(x in BLACKLISTED_OPCODES for x in opcodes):
        print('Forbidden pickle!!')
        exit()
except Exception as e:
    print("That doesn't look like a pickle to me!")
    exit()

print('Here you go:', pickle.loads(p))
