# warmup

There are some bugs exploited in binja/ghidra/ida, you can read about them in my [blog post](https://blog.es3n1n.eu/posts/obfuscator-pt-1/).

tl;dr

decompile in binja because its the only decompiler that can decompile this thing
```cpp
00001830  int32_t main(int32_t argc, char** argv, char** envp)
00001830  {
00001834      int32_t var_54 = 1;
0000186c      printf("flag: ");
00001888      var_20_88;
00001888      __builtin_memset(&var_20_88, 0, 0x19);
000018b5      int64_t s;
000018b5      __builtin_memset(&s, 0, 0x24);
000018cc      __isoc99_scanf("%64s", &s);
00001929      int64_t var_50;
00001929      if ((((int32_t)s) - (0xffffffe9 + 0x1c)) != (0x49 + 0x13))
00001920      {
00001943          var_50 = -1;
00001943      }
0000192e      else
0000192e      {
0000192e          var_50 = 1;
0000192e      }
00001999      int64_t var_50_1;
00001999      if ((((int32_t)s) ^ (((int32_t)*(uint8_t*)((char*)s)[1]) - (3 + 6))) != (4 + 6))
00001990      {
000019b3          var_50_1 = (var_50 - 1);
000019b0      }
0000199e      else
0000199e      {
0000199e          var_50_1 = (var_50 + 1);
0000199b      }
00001a0e      int64_t var_50_2;
00001a0e      if ((((int32_t)*(uint8_t*)((char*)s)[1]) ^ (((int32_t)*(uint8_t*)((char*)s)[2]) - (1 + 0x17))) != (0x4f + 0x18))
00001a05      {
00001a28          var_50_2 = (var_50_1 - 1);
00001a25      }
00001a13      else
00001a13      {
00001a13          var_50_2 = (var_50_1 + 1);
00001a10      }
00001a83      int64_t var_50_3;
// ...
0000296d      char* const format = "nope\n";
00002974      if (rax_69 == ((uint64_t)(6 + 0x1d)))
0000295f      {
00002974          format = "that's right!\n";
00002974      }
0000297a      printf(format);
00002985      return 0;
00002985  }
```

making a list of these comparasions, with replaced neg imms
```py
In [1]: checks = '''
   ...: 00001929      if ((((int32_t)s) - (-23 + 0x1c)) != (0x49 + 0x13))
   ...: 00001999      if ((((int32_t)s) ^ (((int32_t)*(uint8_t*)((char*)s)[1]) - (3 + 6))) != (4 + 6))
   ...: 00001a0e      if ((((int32_t)*(uint8_t*)((char*)s)[1]) ^ (((int32_t)*(uint8_t*)((char*)s)[2]) - (1 + 0x17))) != (0x4f + 0x18))
   ...: 00001a83      if ((((int32_t)*(uint8_t*)((char*)s)[2]) ^ (((int32_t)*(uint8_t*)((char*)s)[3]) - (-13 + 0x19))) != (0x4b + 0x11))
   ...: 00001af8      if ((((int32_t)*(uint8_t*)((char*)s)[3]) ^ (((int32_t)*(uint8_t*)((char*)s)[4]) - (1 + 0xa))) != (8 + 0x1a))
   ...: 00001b68      if ((((int32_t)*(uint8_t*)((char*)s)[4]) ^ (((int32_t)*(uint8_t*)((char*)s)[5]) - (-13 + 0x1c))) != (0x27 + 0x1c))
   ...: 00001bdd      if ((((int32_t)*(uint8_t*)((char*)s)[5]) ^ (((int32_t)*(uint8_t*)((char*)s)[6]) - (0x10 + 0xd))) != (0x54 + 0xb))
   ...: 00001c52      if ((((int32_t)*(uint8_t*)((char*)s)[6]) ^ (((int32_t)*(uint8_t*)((char*)s)[7]) - (-10 + 0xf))) != (-27 + 0x1a))
   ...: 00001cc7      if ((((int32_t)*(uint8_t*)((char*)s)[7]) ^ (((int32_t)var_40) - (-6 + 0xe))) != (0x16 + 0xb))
   ...: 00001d3c      if ((((int32_t)var_40) ^ (((int32_t)*(uint8_t*)((char*)var_40)[1]) - (-5 + 9))) != (0x27 + 8))
   ...: 00001db1      if ((((int32_t)*(uint8_t*)((char*)var_40)[1]) ^ (((int32_t)*(uint8_t*)((char*)var_40)[2]) - (0xf + 0xc))) != (0x18 + 9))
   ...: 00001e26      if ((((int32_t)*(uint8_t*)((char*)var_40)[2]) ^ (((int32_t)*(uint8_t*)((char*)var_40)[3]) - (-22 + 0x1b))) != (0x34 + 8))
   ...: 00001e9b      if ((((int32_t)*(uint8_t*)((char*)var_40)[3]) ^ (((int32_t)*(uint8_t*)((char*)var_40)[4]) - (-13 + 0x14))) != (0x6c + 0xb))
   ...: 00001f10      if ((((int32_t)*(uint8_t*)((char*)var_40)[4]) ^ (((int32_t)*(uint8_t*)((char*)var_40)[5]) - (9 + 0xd))) != (0x60 + 7))
   ...: 00001f88      if ((((int32_t)*(uint8_t*)((char*)var_40)[5]) ^ (((int32_t)*(uint8_t*)((char*)var_40)[6]) - (-2 + 6))) != (0x19 + 0x14))
   ...: 00002000      if ((((int32_t)*(uint8_t*)((char*)var_40)[6]) ^ (((int32_t)*(uint8_t*)((char*)var_40)[7]) - (-6 + 0xf))) != (0x5b + 9))
   ...: 00002078      if ((((int32_t)*(uint8_t*)((char*)var_40)[7]) ^ (((int32_t)var_38) - (0x12 + 6))) != (0x4f + 0xe))
   ...: 000020f0      if ((((int32_t)var_38) ^ (((int32_t)*(uint8_t*)((char*)var_38)[1]) - (0xe + 8))) != (0x6b + 0x13))
   ...: 00002168      if ((((int32_t)*(uint8_t*)((char*)var_38)[1]) ^ (((int32_t)*(uint8_t*)((char*)var_38)[2]) - (-10 + 0x1b))) != (0x72 + 0xa))
   ...: 000021e0      if ((((int32_t)*(uint8_t*)((char*)var_38)[2]) ^ (((int32_t)*(uint8_t*)((char*)var_38)[3]) - (7 + 0xa))) != (0x4c + 0x1e))
   ...: 00002258      if ((((int32_t)*(uint8_t*)((char*)var_38)[3]) ^ (((int32_t)*(uint8_t*)((char*)var_38)[4]) - (-9 + 0x19))) != (0x1d + 0x14))
   ...: 000022d0      if ((((int32_t)*(uint8_t*)((char*)var_38)[4]) ^ (((int32_t)*(uint8_t*)((char*)var_38)[5]) - (0xd + 7))) != (0x63 + 8))
   ...: 00002348      if ((((int32_t)*(uint8_t*)((char*)var_38)[5]) ^ (((int32_t)*(uint8_t*)((char*)var_38)[6]) - (-21 + 0x1e))) != (0x55 + 0x1d))
   ...: 000023c0      if ((((int32_t)*(uint8_t*)((char*)var_38)[6]) ^ (((int32_t)*(uint8_t*)((char*)var_38)[7]) - (0xf + 7))) != (0x4a + 5))
   ...: 00002438      if ((((int32_t)*(uint8_t*)((char*)var_38)[7]) ^ (((int32_t)var_30) - (4 + 0x19))) != (0x5b + 0xb))
   ...: 000024b0      if ((((int32_t)var_30) ^ (((int32_t)*(uint8_t*)((char*)var_30)[1]) - (-3 + 0x1c))) != (0x23 + 7))
   ...: 00002528      if ((((int32_t)*(uint8_t*)((char*)var_30)[1]) ^ (((int32_t)*(uint8_t*)((char*)var_30)[2]) - (0x14 + 6))) != (0x18 + 0x1e))
   ...: 000025a0      if ((((int32_t)*(uint8_t*)((char*)var_30)[2]) ^ (((int32_t)*(uint8_t*)((char*)var_30)[3]) - (-11 + 0x12))) != (0x1a + 0x18))
   ...: 0000260c      if ((((int32_t)*(uint8_t*)((char*)var_30)[3]) ^ (((int32_t)*(uint8_t*)((char*)var_30)[4]) - (0xe + 8))) != (0x67 + 5))
   ...: 00002684      if ((((int32_t)*(uint8_t*)((char*)var_30)[4]) ^ (((int32_t)*(uint8_t*)((char*)var_30)[5]) - (1 + 0x13))) != (0x53 + 0x16))
   ...: 000026fc      if ((((int32_t)*(uint8_t*)((char*)var_30)[5]) ^ (((int32_t)*(uint8_t*)((char*)var_30)[6]) - (7 + 0xb))) != (0x18 + 0xa))
   ...: 00002774      if ((((int32_t)*(uint8_t*)((char*)var_30)[6]) ^ (((int32_t)*(uint8_t*)((char*)var_30)[7]) - (-8 + 0x15))) != (-24 + 0x17))
   ...: 000027e7      if ((((int32_t)*(uint8_t*)((char*)var_30)[7]) ^ (((int32_t)var_28) - (-6 + 0x10))) != (-10 + 0x10))
   ...: 0000285f      if ((((int32_t)var_28) ^ (((int32_t)*(uint8_t*)((char*)var_28)[1]) - (0x12 + 0xb))) != (0x36 + 0x19))
   ...: 000028cb      if ((((int32_t)*(uint8_t*)((char*)var_28)[1]) ^ (((int32_t)*(uint8_t*)((char*)var_28)[2]) - (9 + 0xd))) != (-8 + 0x19))
   ...: 00002940      if ((((int32_t)*(uint8_t*)((char*)var_28)[2]) ^ (((int32_t)*(uint8_t*)((char*)var_28)[3]) - (2 + 7))) != (0x11 + 9))
   ...: '''.strip().splitlines()

In [2]:
```

what it does is `(ch - imm) ^ prev_ch_or_0` \
the result and xor key is actually wrong, we should add +1 to their values because of the `adc` stuff

```py
In [2]: from z3 import *

In [3]: s = Solver(); s.reset()
    ...: for i, check in enumerate(checks):
    ...:     expr = flag[i]
    ...:     expr -= BitVecVal(eval(check.split('- (')[1].split(')')[0]+'+1'), 32)
    ...:     if '^' in check:
    ...:         expr ^= flag[i - 1]
    ...:     s.add(expr == BitVecVal(eval(check.split('!= (')[1].split(')')[0]+'+1'), 32))
    ...:

In [4]: s.check()
Out[4]: sat

In [5]: ''.join([chr(s.model()[x].as_long()) for x in flag])
Out[5]: 'cr3{d0nt_trU5t_D3c0mp1L3rs_t0o_muCh}'

In [6]:
```
