# messed-up-calculation


Disassembly explained
```cpp
int __cdecl main(int argc, const char **argv, const char **envp)
{
  __int64 v3; // r14
  __int64 v4; // rax
  char v5; // r15
  __int64 v6; // r12
  unsigned __int64 v7; // rdx
  __int64 v8; // rax
  __int64 i; // rcx
  unsigned __int64 v10; // rsi
  __int64 v11; // rdi
  __int64 v12; // r12
  __int64 v13; // rbx
  __int64 v14; // rax
  char v15; // bp
  __int64 v16; // rsi
  __int64 v18; // [rsp+0h] [rbp-A28h] BYREF
  unsigned __int64 v19; // [rsp+8h] [rbp-A20h]
  __int64 v20[3]; // [rsp+10h] [rbp-A18h] BYREF
  __int64 v21; // [rsp+28h] [rbp-A00h]
  __int128 s; // [rsp+30h] [rbp-9F8h] BYREF
  void *v23; // [rsp+40h] [rbp-9E8h]
  __int64 v24; // [rsp+9F0h] [rbp-38h]

  v3 = std::__put_character_sequence[abi:v170002]<char,std::char_traits<char>>(
         &std::cout,
         "Hallo, please wait while im looking for a seed.",
         47LL);
  std::ios_base::getloc((std::ios_base *)&s);
  v4 = std::locale::use_facet(&s, &std::ctype<char>::id);
  v5 = (*(__int64 (__fastcall **)(__int64, __int64))(*(_QWORD *)v4 + 56LL))(v4, 10LL);
  std::locale::~locale((std::locale *)&s);
  std::ostream::put(v3, (unsigned int)v5);
  std::ostream::flush(v3);
  do
  {
    LOBYTE(s) = 24;
    strcpy((char *)&s + 1, "/dev/urandom");
    std::random_device::random_device(&v18, &s);
    if ( (s & 1) != 0 )
      operator delete(v23);
    v6 = (unsigned int)std::random_device::operator()(&v18);
    v7 = (v6 << 32) | (unsigned int)std::random_device::operator()(&v18);
    *(_QWORD *)&s = v7;
    v8 = 1LL;
    for ( i = 2LL; ; i += 2LL )
    {
      v10 = i + 0x5851F42D4C957F2DLL * (v7 ^ (v7 >> 62)) - 1;
      *(&v21 + i) = v10;
      if ( i == 312 )
        break;
      v11 = 0x5851F42D4C957F2DLL * (v10 ^ (v10 >> 62));
      v7 = v11 + v8 + 1;
      *((_QWORD *)&s + i) = i + v11;
      v8 += 2LL;
    }
    v24 = 0LL;
    v20[0] = 0x8000000000000000LL;
    v20[1] = 0x7FFFFFFFFFFFFFFFLL;
    v12 = std::uniform_int_distribution<long>::operator()<std::mersenne_twister_engine<unsigned long,64ul,312ul,156ul,31ul,13043109905998158313ul,29ul,6148914691236517205ul,17ul,8202884508482404352ul,37ul,18444473444759240704ul,43ul,6364136223846793005ul>>(
            v20,
            &s,
            v20);
    std::random_device::~random_device((std::random_device *)&v18);
    v21 = v12;
    v20[2] = 0xDE47307EBB83312ALL;
    /// v12 - random number
    v20[0] = ~(v12 & 0xDE47307EBB83312ALL);
    v18 = ~(v20[0] & v12);
    v19 = ~(v20[0] & 0xDE47307EBB83312ALL);
    v20[0] = v19;
    *(_QWORD *)&s = v19 & v18;
  }
  while ( (v19 & v18) != 0x42CAFCFA134CB9A8LL );

  v13 = std::__put_character_sequence[abi:v170002]<char,std::char_traits<char>>(
          &std::cout,
          "Seed has been found! Here is your flag: ",
          40LL);
  std::ios_base::getloc((std::ios_base *)&s);
  v14 = std::locale::use_facet(&s, &std::ctype<char>::id);
  v15 = (*(__int64 (__fastcall **)(__int64, __int64))(*(_QWORD *)v14 + 56LL))(v14, 10LL);
  std::locale::~locale((std::locale *)&s);
  v16 = (unsigned int)v15;
  std::ostream::put(v13, v16);
  std::ostream::flush(v13);

  /// function that generates a number 
  v20[0] = `anonymous namespace'::OPTIMIZE_ME((_anonymous_namespace_ *)v12);

  /// Number is then converted with some operations
  *(_QWORD *)&s = __ROL8__(v20[0], 20) ^ 0x104C720B3B3F960DLL;
  *((_QWORD *)&s + 1) = __ROL8__(v20[0], 6) ^ 0xEC98E4A44683346CLL;
  puts((const char *)&s);
  return 0;
}
```

First, let's find a number that it is looking for with random number generator
```py
In [1]: from z3 import *

In [2]: s = Solver()

In [3]: x = BitVec('x', 64)

In [4]: y = ~(x & 0xDE47307EBB83312A)

In [5]: v18 = ~(y & x)

In [6]: v19 = ~(y & 0xDE47307EBB83312A)

In [7]: s.add((v19 & v18) == 0x42CAFCFA134CB9A8)

In [8]: assert s.check() == sat

In [9]: print(hex(s.model()[x].as_long()))
0x6372337b5730777d

In [10]:
```

Now we know that it passes this value to the OPTIMIZE_ME function, let's take a look at it.
```cpp
__int64 __fastcall `anonymous namespace'::OPTIMIZE_ME(_anonymous_namespace_ *this)
{
  __int64 i; // [rsp+0h] [rbp-18h]
  __int64 v3; // [rsp+8h] [rbp-10h]

  v3 = 0LL;
  for ( i = 0LL; i < (__int64)this; ++i )
  {
    if ( (i & 1) != 0 )
    {
      if ( (i & 1) == 1 )
        v3 -= i;
      else
        v3 = 0LL;
    }
    else
    {
      v3 += i;
    }
  }
  return v3;
}
```

Simplifying it to
```py
In [10]: round(0x6372337B5730777D / 2) * -1
Out[10]: -3582923280968334336

In [11]:
```

Please note that instead of operating with floating point number in python we should've used `//` operator. 

Now we need to do these operations and we will get the flag
```cpp
  *(_QWORD *)&s = __ROL8__(v20[0], 20) ^ 0x104C720B3B3F960DLL;
  *((_QWORD *)&s + 1) = __ROL8__(v20[0], 6) ^ 0xEC98E4A44683346CLL;
```

Implementation:
```cpp
	std::int64_t val = -3582923280968334336;
	
	std::array<std::int64_t, 3> values = {};
	
	values[0] = (int64_t)std::rotl((uint64_t)val, 20);
	values[0] ^= 0x104C720B3B3F960DLL;
	
	values[1] = (int64_t)std::rotl((uint64_t)val, 6);
	values[1] ^= 0xEC98E4A44683346CLL;

	printf("%s\n", values.data());
```

Which prints
```
cr3{w4it_4r_1t!}
```
