# decrypt-me

In this binary there are a bunch of math, let's break down what it does.

First thing first, it saves the rdtsc values
```cpp
    seeds_u32 = operator new(0x10uLL);
    v103 = &`vtable for'std::__narrow_to_utf8<32ul> + 16;
    seeds_u32[1] = 0LL;
    v101 = seeds_u32;
    *seeds_u32 = __rdtsc();
```

If we take a look at this bunch of math stuff and google some constants from it(`0x6C078965`, `0x9D2C5680`),
we'll find in Google that we are looking at the mersenne twister pseudorandom generator.

Knowing that, let's continue breaking down the logic of this thing.

Here is how it initializes the mt19937 seed (by taking a random value from urandom)
```cpp
    strcpy(v87 + 1, "/dev/urandom");
    std::random_device::random_device(v89, v87);
    v23 = v101;
    if ( (v87[0] & 1) != 0 )
    operator delete(v88);
    prng_seed = std::random_device::operator()(v89);
    prng[0] = prng_seed;
    v25 = 2LL;
    v26 = prng_seed;
```

Then its doing mod 1337 from this seed and storing it somewhere. 
```cpp
    seed_2 = *v23;
    _1337 = _byteswap_uint64(0x3905000000000000uLL);// 1337
    if ( (_1337 | seed_2) >> 32 )
    divided_stuff = seed_2 % _1337;
    else
    divided_stuff = seed_2 % _1337;
```

Then its storing 
```cpp
      *(HEADER + 2) = (LODWORD(prng[0]) ^ (prng[0] >> 11) ^ ((LODWORD(prng[0]) ^ (prng[0] >> 11)) << 7) & 0x9D2C5680 ^ ((LODWORD(prng[0]) ^ (prng[0] >> 11) ^ ((LODWORD(prng[0]) ^ (prng[0] >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000 ^ ((prng[0] ^ (prng[0] >> 11) ^ ((LODWORD(prng[0]) ^ (prng[0] >> 11)) << 7) & 0x9D2C5680 ^ (((LODWORD(prng[0]) ^ (prng[0] >> 11) ^ ((LODWORD(prng[0]) ^ (prng[0] >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000)) >> 18))
                    + (flag_end - flag_start) / 7u;
      qword_A060 = v33 ^ -(v32 & 1) & 0x9908B0DF;
      v34 = qword_A060 ^ (qword_A060 >> 11) ^ ((qword_A060 ^ (qword_A060 >> 11)) << 7) & 0x9D2C5680 ^ ((qword_A060 ^ (qword_A060 >> 11) ^ ((qword_A060 ^ (qword_A060 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000;
      v35 = qword_A070;
      qword_A068 = qword_ACD0 ^ ((v32 & 0xFFFFFFFF80000000LL | qword_A070 & 0x7FFFFFFE) >> 1) ^ -(qword_A070 & 1) & 0x9908B0DF;
      v36 = qword_A068 ^ (qword_A068 >> 11) ^ ((qword_A068 ^ (qword_A068 >> 11)) << 7) & 0x9D2C5680;
      *(HEADER + 3) = ((v34 ^ v36 ^ ((v36 << 15) & 0xEFC60000)) >> 18) ^ v34 ^ v36 ^ (v36 << 15) & 0xEFC60000;
      qword_A070 = qword_ACD8 ^ ((v35 & 0xFFFFFFFF80000000LL | qword_A078 & 0x7FFFFFFE) >> 1) ^ -(qword_A078 & 1) & 0x9908B0DF;
      v37 = qword_A070 ^ (qword_A070 >> 11) ^ ((qword_A070 ^ (qword_A070 >> 11)) << 7) & 0x9D2C5680;
      qword_A078 = qword_ACE0 ^ ((qword_A078 & 0xFFFFFFFF80000000LL | qword_A080 & 0x7FFFFFFE) >> 1) ^ -(qword_A080 & 1) & 0x9908B0DF;
      v38 = qword_A088;
      v39 = qword_ACE8 ^ ((qword_A080 & 0xFFFFFFFF80000000LL | qword_A088 & 0x7FFFFFFE) >> 1);
      *HEADER = seed_2 ^ (((v37 ^ (v37 << 15) & 0xEFC60000 ^ ((v37 ^ ((v37 << 15) & 0xEFC60000)) >> 18)) << 32) | qword_A078 ^ (qword_A078 >> 11) ^ ((qword_A078 ^ (qword_A078 >> 11)) << 7) & 0x9D2C5680 ^ ((qword_A078 ^ (qword_A078 >> 11) ^ ((qword_A078 ^ (qword_A078 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000 ^ ((qword_A078 ^ (qword_A078 >> 11) ^ ((qword_A078 ^ (qword_A078 >> 11)) << 7) & 0x9D2C5680 ^ (((qword_A078 ^ (qword_A078 >> 11) ^ ((qword_A078 ^ (qword_A078 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000)) >> 18));
```

Which is essentially just a
```cpp
header + 0 = (u64)rdtsc ^ (u64)random_number();
header + 8  = (u32)random_number() + (flag_len / 7);
header + 12 = (u32)random_number() ^ random_number();
```

These values are later written to the output file, so we can easily obtain it.
```python
with open('./flag.enc.bin', 'rb') as f:
    encrypted = f.read()

seed_1 = int.from_bytes(encrypted[0:8], 'little')
seed_2 = int.from_bytes(encrypted[8:12], 'little')
seed_3 = int.from_bytes(encrypted[12:16], 'little')
```

Then it generates a bunch of random numbers again.
```cpp
      SOME_ARRAY[0] = v40 ^ (v40 >> 18);
      qword_A088 = qword_ACF0 ^ ((v38 & 0xFFFFFFFF80000000LL | qword_A090 & 0x7FFFFFFE) >> 1) ^ -(qword_A090 & 1) & 0x9908B0DF;
      v41 = qword_A088 ^ (qword_A088 >> 11) ^ ((qword_A088 ^ (qword_A088 >> 11)) << 7) & 0x9D2C5680;
      SOME_ARRAY[1] = v41 ^ (v41 << 15) & 0xEFC60000 ^ ((v41 ^ ((v41 << 15) & 0xEFC60000)) >> 18);
      qword_A090 = qword_ACF8 ^ ((qword_A090 & 0xFFFFFFFF80000000LL | qword_A098 & 0x7FFFFFFE) >> 1) ^ -(qword_A098 & 1) & 0x9908B0DF;
      SOME_ARRAY[2] = qword_A090 ^ (qword_A090 >> 11) ^ ((qword_A090 ^ (qword_A090 >> 11)) << 7) & 0x9D2C5680 ^ ((qword_A090 ^ (qword_A090 >> 11) ^ ((qword_A090 ^ (qword_A090 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000 ^ ((qword_A090 ^ (qword_A090 >> 11) ^ ((qword_A090 ^ (qword_A090 >> 11)) << 7) & 0x9D2C5680 ^ (((qword_A090 ^ (qword_A090 >> 11) ^ ((qword_A090 ^ (qword_A090 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000)) >> 18);
      qword_A098 = qword_AD00 ^ ((qword_A098 & 0xFFFFFFFF80000000LL | qword_A0A0 & 0x7FFFFFFE) >> 1) ^ -(qword_A0A0 & 1) & 0x9908B0DF;
      SOME_ARRAY[3] = qword_A098 ^ (qword_A098 >> 11) ^ ((qword_A098 ^ (qword_A098 >> 11)) << 7) & 0x9D2C5680 ^ ((qword_A098 ^ (qword_A098 >> 11) ^ ((qword_A098 ^ (qword_A098 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000 ^ ((qword_A098 ^ (qword_A098 >> 11) ^ ((qword_A098 ^ (qword_A098 >> 11)) << 7) & 0x9D2C5680 ^ (((qword_A098 ^ (qword_A098 >> 11) ^ ((qword_A098 ^ (qword_A098 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000)) >> 18);
      qword_A0A0 = qword_AD08 ^ ((qword_A0A0 & 0xFFFFFFFF80000000LL | qword_A0A8 & 0x7FFFFFFE) >> 1) ^ -(qword_A0A8 & 1) & 0x9908B0DF;
      qword_B3D8 = 10LL;
```

One more.
```cpp
      v42 = qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) & 0x9D2C5680 ^ ((qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000 ^ ((qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) & 0x9D2C5680 ^ (((qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000)) >> 18);
      v43 = 32;
      if ( (qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) & 0x9D2C5680 ^ (((qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000)) >> 18)) >= 0x21u )
        v43 = (qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) & 0x9D2C5680 ^ (((qword_A0A0 ^ (qword_A0A0 >> 11) ^ ((qword_A0A0 ^ (qword_A0A0 >> 11)) << 7) & 0x9D2C5680) << 15) & 0xEFC60000)) >> 18));
      v44 = 64;
      if ( v43 < 0x40 )
        v44 = v43;
```

But it stores not only the value itself, but also clamps it to a different stack variable.
```cpp
min(max(value & 0xFF, 32U), 64U)
```

Then its encrypting it with XTEA using the `SOME_ARRAY` as key and previous values as delta, rounds.
```cpp
        v45 = v31 >> 3;
        v46 = 0LL;
        do
        {
          v47 = v30[2 * v46];
          v48 = v30[2 * v46 + 1];
          v49 = v42;
          v50 = v44;
          v51 = 0;
          do
          {
            v47 += (v48 + ((16 * v48) ^ (v48 >> 5))) ^ (v51 + SOME_ARRAY[v51 & 3]);
            v51 += v42;
            v48 += (v47 + ((16 * v47) ^ (v47 >> 5))) ^ (v49 + SOME_ARRAY[(v49 >> 11) & 3]);
            v49 += v42;
            --v50;
          }
          while ( v50 );
          v30[2 * v46] = v47;
          v30[2 * v46++ + 1] = v48;
        }
        while ( v46 != v45 );
```

Then it enters a loop from 0 to the divided number.
```cpp
if ( divided_stuff )
{
    seed_2_and = seed_2 & 7u;
    i = 0LL;
    do
    {
      ...
      ++i;
    }
    while ( i < divided_stuff );
}
```

There it gets another random number and iterates from 0 to it.
```cpp
range = seed_2_and ^ ((((v56 ^ ((v56 ^ (v56 << 7) & 0x12C0000 ^ ((v56 & 8u) << 15)) >> 18)) & 0xF)
                               * ((v59 ^ (v59 >> 11) ^ ((v59 ^ (v59 >> 11) ^ ((v59 ^ (v59 >> 11)) << 7) & 0x12C0000 ^ (((v59 ^ (v59 >> 11)) & 8) << 15)) >> 18)) & 7))
                              % (((v61 ^ (v61 >> 11) ^ ((v61 ^ (v61 >> 11) ^ ((v61 ^ (v61 >> 11)) << 7) & 0x12C0000 ^ (((v61 ^ (v61 >> 11)) & 8) << 15)) >> 18)) & 0xF)
                               + 5));
```

Which is essentially:
```cpp
range = seed_2_and ^ ((rand_num() & 0xF) * (random_num() & 7) % (rand_num() + 5);
```

Then in this loop.
```cpp
for ( j = 0LL; j != range; ++j )
{
  v64 = flag_start;
  v65 = flag_end;
  while ( v64 != v65 )
  {
    v67 = qword_B3D8 - 624 * ((qword_B3D8 + 1) / 0x270uLL) + 1;
    prng[qword_B3D8] = prng[qword_B3D8 - 624 * ((qword_B3D8 + 397) / 0x270uLL) + 397] ^ ((prng[qword_B3D8] & 0xFFFFFFFF80000000LL | prng[v67] & 0x7FFFFFFE) >> 1) ^ -(prng[v67] & 1) & 0x9908B0DF;
    v68 = prng[qword_B3D8] ^ (prng[qword_B3D8] >> 11);
    qword_B3D8 = v67;
    switch ( ((v68 ^ (v68 << 7)) ^ ((v68 ^ (v68 << 7) & 0x9D2C5680 ^ ((v68 ^ (v68 << 7) & 0x9D2C5680) << 15) & 0x3C40000) >> 18))
           - 3
           * (((v68 ^ (v68 << 7) ^ ((v68 ^ (v68 << 7) & 0x9D2C5680 ^ ((v68 ^ (v68 << 7) & 0x9D2C5680) << 15) & 0x3C40000) >> 18))
             / 3u) & 254) )
    {
```

Simplified:
```cpp
for ( j = 0LL; j != range; ++j )
{
  v64 = flag_start;
  v65 = flag_end;
  while ( v64 != v65 )
  {
    switch ( random_num() % 6 )
    {
```

Then in this switch it applies some math operations to the flag
```cpp
case 0u:
    v69 = v67 - 623;
    if ( v67 < 0x26F )
      v69 = v67 + 1;
    v70 = v67 - 227;
    if ( v67 < 0xE3 )
      v70 = v67 + 397;
    v71 = prng[v70] ^ ((prng[v67] & 0xFFFFFFFF80000000LL | prng[v69] & 0x7FFFFFFE) >> 1) ^ -(prng[v69] & 1) & 0x9908B0DF;
    prng[v67] = v71;
    v72 = v71 ^ (v71 >> 11);
    qword_B3D8 = v69;
    v66 = *v64 ^ v72 ^ (v72 << 7) ^ ((v72 ^ (v72 << 7) & 0x9D2C5680 ^ ((v72 ^ (v72 << 7) & 0x9D2C5680) << 15) & 0x3C40000) >> 18);
    break;
case 1u:
    v77 = v67 - 623;
    if ( v67 < 0x26F )
      v77 = v67 + 1;
    v78 = v67 - 227;
    if ( v67 < 0xE3 )
      v78 = v67 + 397;
    v79 = prng[v78] ^ ((prng[v67] & 0xFFFFFFFF80000000LL | prng[v77] & 0x7FFFFFFE) >> 1) ^ -(prng[v77] & 1) & 0x9908B0DF;
    prng[v67] = v79;
    v80 = v79 ^ (v79 >> 11);
    qword_B3D8 = v77;
    v66 = *v64
        + (v80 ^ (v80 << 7) ^ ((v80 ^ (v80 << 7) & 0x9D2C5680 ^ ((v80 ^ (v80 << 7) & 0x9D2C5680) << 15) & 0x3C40000) >> 18));
    break;
case 2u:
    v73 = v67 - 623;
    if ( v67 < 0x26F )
      v73 = v67 + 1;
    v74 = v67 - 227;
    if ( v67 < 0xE3 )
      v74 = v67 + 397;
    v75 = prng[v74] ^ ((prng[v67] & 0xFFFFFFFF80000000LL | prng[v73] & 0x7FFFFFFE) >> 1) ^ -(prng[v73] & 1) & 0x9908B0DF;
    prng[v67] = v75;
    v76 = v75 ^ (v75 >> 11);
    qword_B3D8 = v73;
    v66 = *v64
        - (v76 ^ (v76 << 7) ^ ((v76 ^ (v76 << 7) & 0x9D2C5680 ^ ((v76 ^ (v76 << 7) & 0x9D2C5680) << 15) & 0x3C40000) >> 18));
    break;
case 3u:
    v66 = __ROL1__(*v64, 1);
    break;
case 4u:
    v66 = __ROR1__(*v64, 1);
    break;
case 5u:
    v66 = ~*v64;
    break;
default:
    std::terminate();
```

Simplified:
```cpp
case 0u:
    ch ^= random_num() & 0xFF;
    break;
case 1u:
    ch += random_num() & 0xFF;
    break;
case 2u:
    ch -= random_num() & 0xFF;
    break;
case 3u:
    ch = __ROL1__(ch, 1);
    break;
case 4u:
    ch = __ROR1__(ch, 1);
    break;
case 5u:
    ch = ~ch;
    break;
default:
    std::terminate();
```

Then it copies all of this to a buffer and writes to file.

## Decrypting

To decrypt this, we first need to find the mersenne twister seed, we can use a modified version of 
[RNGeesus](https://github.com/deut-erium/RNGeesus) for that.

```python
br = Breaker()
s = Solver()

output_some_1 = BitVec('output_some_1', 32)
output_some_2 = BitVec('output_some_2', 32)
output_some_3 = BitVec('output_some_3', 32)

xxt = output_some_1 + (len(encrypted) // 7)
xxd = output_some_2 ^ output_some_3

s.add(xxt == seed_2)
s.add(xxd == seed_3)

print('[+] recovering seed')
recovered_seed = br.get_seed_mt(s, [
    (0, output_some_1),
    (1, output_some_2),
    (2, output_some_3),
])
assert recovered_seed is not None
```

Then let's extract the xtea keys and such random things.
```python
mt = MT19937(c_seed=recovered_seed)
[mt.extract_number() for _ in range(3)]

vvv = (mt.extract_number() << 32) | mt.extract_number()
orig_seed = seed_1 ^ vvv
print('[+] original seed:', hex(orig_seed))

keys = [
    mt.extract_number(),
    mt.extract_number(),
    mt.extract_number(),
    mt.extract_number(),
]
print('[+] keys:', ';'.join(map(hex, keys)))

delta = mt.extract_number()
print('[+] delta:', hex(delta))

num_rounds = min(max(delta & 0xFF, 32), 64)
print('[+] num_rounds:', hex(num_rounds))
```

Then we have to reverse these math operations.
```python
print('[+] reverting pt.1')
operations = []
for i in range(orig_seed % 1337):
    v1 = (mt.extract_number() & 0xF) * (mt.extract_number() & 0b111)
    pp = v1 % ((mt.extract_number() & 0xF) + 5)
    pp ^= uint8(orig_seed & 0b111)
    pp &= 0xFF
    for k in range(pp):
        for j in range(len(encrypted)):
            n = (mt.extract_number() & 0xFF) % 6

            imm = 0
            if n != 3 and n != 4 and n != 5:
                imm = mt.extract_number()

            operations.append([n, j, imm])

print(f'[+] {len(operations)} operations to revert')
for (n, j, imm) in reversed(operations):
    if n == 0:
        encrypted[j] = encrypted[j] ^ (imm & 0xFF)
    elif n == 1:
        encrypted[j] = (encrypted[j] - (imm & 0xFF)) & 0xFF
    elif n == 2:
        encrypted[j] = (encrypted[j] + (imm & 0xFF)) & 0xFF
    elif n == 3:
        encrypted[j] = rotr8(encrypted[j], 1)
    elif n == 4:
        encrypted[j] = rotl8(encrypted[j], 1)
    elif n == 5:
        encrypted[j] = (~uint8(encrypted[j])) & 0xFF
    else:
        raise ValueError()
```

Afterwards, let's decrypt xtea and we're done.
```python
def xtea_decrypt_int(k, v, delta_v=0x9e3779b9, rounds=32):
    v0, v1 = v

    delta, mask = delta_v, 0xffffffff
    sum = (delta * rounds) & mask
    for _ in range(rounds):
        v1 = (v1 - (((v0 << 4 ^ v0 >> 5) + v0) ^
                    (sum + k[sum >> 11 & 3]))) & mask
        sum = (sum - delta) & mask
        v0 = (v0 - (((v1 << 4 ^ v1 >> 5) + v1) ^
                    (sum + k[sum & 3]))) & mask

    return v0, v1


print('[+] reverting pt.2')
flag = []

for i in range(len(encrypted) // 8):
    raw = encrypted[i * 8:(i + 1) * 8]

    vals = [
        int.from_bytes(raw[:4], 'little'),
        int.from_bytes(raw[4:], 'little'),
    ]

    flag = [*flag, *xtea_decrypt_int(keys, vals, delta, num_rounds)]

flag_str = ''
for x in flag:
    flag_str += x.to_bytes(4, 'little').decode()

print('[+] flag:', flag_str)
```
