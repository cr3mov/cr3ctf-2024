from flask import Flask, request
import numpy as np

import json
import traceback

app = Flask(__name__)

KEY = 28308
XOR = -578794842304260765


def encrypt_str(check):
    meme2 = [KEY]
    i4 = 1
    while True:
        if i4 > len(check):
            break
        meme2.append((check[i4 - 1] ^ (i4 * meme2[0]) ^ XOR) & 0xffff)
        i4 += 1
    return "".join([chr(x) for x in meme2]).encode(errors="surrogatepass")


def rotl32(n, d, INT_BITS=32):
    l = np.uint32(n) << np.uint32(d)
    r = np.uint32(n) >> np.uint32(INT_BITS - d)
    return np.uint32(l | r)


def rotl64(n, d, INT_BITS=64):
    l = np.uint64(n) << np.uint64(d)
    r = np.uint64(n) >> np.uint64(INT_BITS - d)
    return np.uint64(l | r)


def dumb_behavior_x86(c):
    if c & 0x80:
        return np.uint32(0xffffff00 | c)
    return np.uint32(c)


def mmh3_32(data, seed):
    hash = np.uint32(seed)
    blocks = len(data) >> 2

    for i in range(blocks):
        i_4 = i << 2
        k = np.uint32(data[i_4] | (data[i_4 + 1] << 8) | (data[i_4 + 2] << 16) | (data[i_4 + 3] << 24))

        k = np.uint32(rotl32(k * np.uint32(0xCC9E2D51), 15))
        k = k * np.uint32(0x1B873593)

        hash = np.uint32(rotl32(hash ^ np.uint32(k), 13))
        hash = hash * np.uint32(5)
        hash = hash + np.uint32(0xE6546B64)

    idx = blocks << 2
    k1 = np.uint32(0)
    rem = len(data) - idx
    if rem == 3:
        k1 = k1 ^ np.uint32(dumb_behavior_x86(data[idx + 2]) << 16)
    if rem >= 2:
        k1 = k1 ^ np.uint32(dumb_behavior_x86(data[idx + 1]) << 8)
    if rem >= 1:
        k1 = k1 ^ dumb_behavior_x86(data[idx])
        k1 = np.uint32(rotl32(k1 * np.uint32(0xCC9E2D51), 15))
        k1 = k1 * np.uint32(0x1B873593)
        hash = hash ^ np.uint32(k1)

    hash = hash ^ np.uint32(len(data))
    hash = hash ^ np.uint32(np.uint32(hash) >> 16)
    hash = hash * np.uint32(0x85ebca6b)
    hash = hash ^ np.uint32(np.uint32(hash) >> 13)
    hash = hash * np.uint32(0xc2b2ae35)
    hash = hash ^ np.uint32(np.uint32(hash) >> 16)

    return int(hash)


def mmh3_64(data, seed):
    hash = np.uint64(seed)
    blocks = len(data) >> 3

    for i in range(blocks):
        i_8 = i << 3
        k = np.uint64(
            data[i_8] |
            (data[i_8 + 1] << 8) |
            (data[i_8 + 2] << 16) |
            (data[i_8 + 3] << 24) |
            (data[i_8 + 4] << 32) |
            (data[i_8 + 5] << 40) |
            (data[i_8 + 6] << 48) |
            (data[i_8 + 7] << 56)
        )

        k = np.uint64(rotl64(k * np.uint64(0x87C37B91114253D5), 31))
        k = k * np.uint64(0x4CF5AD432745937F)

        hash = np.uint64(rotl64(hash ^ np.uint64(k), 27))
        hash = hash * np.uint64(5)
        hash = hash + np.uint64(0x52DCE729)

    k1 = np.uint64(0)
    idx = blocks << 3
    rem = len(data) - idx
    if rem == 7:
        k1 ^= np.uint64(data[idx + 6]) << np.uint64(48)
    if rem >= 6:
        k1 ^= np.uint64(data[idx + 5]) << np.uint64(40)
    if rem >= 5:
        k1 ^= np.uint64(data[idx + 4]) << np.uint64(32)
    if rem >= 4:
        k1 ^= np.uint64(data[idx + 3]) << np.uint64(24)
    if rem >= 3:
        k1 ^= np.uint64(data[idx + 2]) << np.uint64(16)
    if rem >= 2:
        k1 ^= np.uint64(data[idx + 1]) << np.uint64(8)
    if rem >= 1:
        k1 ^= np.uint64(data[idx])
        k1 = rotl64(k1 * np.uint64(0x87C37B91114253D5), 31)
        k1 = k1 * np.uint64(0x4CF5AD432745937F)
        hash ^= k1

    hash ^= np.uint64(len(data))
    hash ^= hash >> np.uint64(33)
    hash = hash * np.uint64(0xff51afd7ed558ccd)
    hash ^= hash >> np.uint64(33)
    hash = hash * np.uint64(0xc4ceb9fe1a85ec53)
    hash ^= hash >> np.uint64(33)

    return int(hash)


def calc_checksum(hi, lo, key):
    lo = lo & 0xffffffff
    i4 = 0
    while i4 < 16:
        v2 = (key << i4) & 0xffffffff
        v3 = (key >> (32 - i4)) & 0xffffffff
        v2 |= v3
        v2 = v2 ^ lo
        hi = hi ^ v2
        i4 += 1
        v4 = lo
        lo = hi
        hi = v4

    return ((hi << 32) | (lo & 0xffffffff)) & 0xffffffffffffffff


@app.route("/login", methods=["POST", "GET"])
def index():
    try:
        j = request.data
        obj = json.loads(j)

        checksum = obj.get('checksum', 0)
        s = encrypt_str(j[:j.find(str(checksum).encode())])
        key = mmh3_32(s, 0)
        hilo = mmh3_64(s, 0)

        if calc_checksum(hilo >> 32, hilo & 0xffffffff, key) != checksum:
            return "Invalid checksum"

        if obj.get('admin', False):
            return "cr3{1_h0p3_y0u_s0lv3d_1t_th3_w4y_1_w4nt3d_t0o}"
        else:
            return "cr3{w0w_y0u_h4v3_f0und_f4k3_fl49}"
    except Exception as e:
        print(traceback.format_exc())
        return "Sorry, we don't deal with retarded users"
