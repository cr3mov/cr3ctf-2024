# Writeup

1. Open in ghidra with gameboy plugin
2. Find this function with MBA by using debugger within the gameboy emulator i provided
```cpp
undefined FUN_062b(undefined2 param_1,byte *param_2)
{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  byte bVar9;
  byte bVar10;
  byte bVar11;
  byte bVar12;
  byte bVar13;
  byte bVar14;
  byte bVar15;
  byte bVar16;
  byte bVar17;
  byte bVar18;
  byte bVar19;
  byte bVar20;
  byte bVar21;
  byte bVar22;
  byte bVar23;
  byte bVar24;
  byte bVar25;
  byte bVar26;
  byte bVar27;
  byte bVar28;
  byte bVar29;
  byte bVar30;
  byte bVar31;
  byte bVar32;
  undefined uVar33;
  byte bVar34;
  byte bVar35;
  
  bVar35 = (byte)((uint)param_1 >> 8);
  bVar34 = (byte)param_1;
  bVar1 = *param_2;
  bVar2 = param_2[1];
  bVar3 = param_2[2];
  bVar4 = param_2[3];
  bVar5 = param_2[4];
  bVar6 = param_2[5];
  bVar7 = param_2[6];
  bVar8 = param_2[7];
  bVar9 = param_2[8];
  bVar10 = param_2[9];
  bVar11 = param_2[10];
  bVar12 = param_2[0xb];
  bVar13 = param_2[0xc];
  bVar14 = param_2[0xd];
  bVar15 = param_2[0xe];
  bVar16 = param_2[0xf];
  bVar17 = ~(~bVar1 & bVar9);
  bVar18 = ~(~bVar2 & bVar10);
  bVar19 = ~(~bVar3 & bVar11);
  bVar20 = ~(~bVar4 & bVar12);
  bVar21 = ~(~bVar5 & bVar13);
  bVar22 = ~(~bVar6 & bVar14);
  bVar23 = ~(~bVar7 & bVar15);
  bVar24 = ~(~bVar8 & bVar16);
  bVar17 = ~(~(bVar17 & bVar9) & ~(~bVar1 & bVar17));
  bVar18 = ~(~(bVar18 & bVar10) & ~(~bVar2 & bVar18));
  bVar19 = ~(~(bVar19 & bVar11) & ~(~bVar3 & bVar19));
  bVar20 = ~(~(bVar20 & bVar12) & ~(~bVar4 & bVar20));
  bVar21 = ~(~(bVar21 & bVar13) & ~(~bVar5 & bVar21));
  bVar22 = ~(~(bVar22 & bVar14) & ~(~bVar6 & bVar22));
  bVar23 = ~(~(bVar23 & bVar15) & ~(~bVar7 & bVar23));
  bVar24 = ~(~(bVar24 & bVar16) & ~(~bVar8 & bVar24));
  bVar25 = ~(bVar17 & ~(bVar1 | ~bVar9));
  bVar26 = ~(bVar18 & ~(bVar2 | ~bVar10));
  bVar27 = ~(bVar19 & ~(bVar3 | ~bVar11));
  bVar28 = ~(bVar20 & ~(bVar4 | ~bVar12));
  bVar29 = ~(bVar21 & ~(bVar5 | ~bVar13));
  bVar30 = ~(bVar22 & ~(bVar6 | ~bVar14));
  bVar31 = ~(bVar23 & ~(bVar7 | ~bVar15));
  bVar32 = ~(bVar24 & ~(bVar8 | ~bVar16));
  bVar17 = ~(~bVar9 | bVar1) | ~(bVar17 & bVar25) & ~(bVar25 & ~(bVar1 | ~bVar9));
  bVar18 = ~(~bVar10 | bVar2) | ~(bVar18 & bVar26) & ~(bVar26 & ~(bVar2 | ~bVar10));
  bVar19 = ~(~bVar11 | bVar3) | ~(bVar19 & bVar27) & ~(bVar27 & ~(bVar3 | ~bVar11));
  bVar20 = ~(~bVar12 | bVar4) | ~(bVar20 & bVar28) & ~(bVar28 & ~(bVar4 | ~bVar12));
  bVar21 = ~(~bVar13 | bVar5) | ~(bVar21 & bVar29) & ~(bVar29 & ~(bVar5 | ~bVar13));
  bVar22 = ~(~bVar14 | bVar6) | ~(bVar22 & bVar30) & ~(bVar30 & ~(bVar6 | ~bVar14));
  bVar23 = ~(~bVar15 | bVar7) | ~(bVar23 & bVar31) & ~(bVar31 & ~(bVar7 | ~bVar15));
  bVar24 = ~(~bVar16 | bVar8) | ~(bVar24 & bVar32) & ~(bVar32 & ~(bVar8 | ~bVar16));
  bVar17 = ~(~(~(bVar17 & bVar34) & bVar34) & ~(~(bVar17 & bVar34) & bVar17));
  bVar18 = ~(~(~(bVar18 & bVar35) & bVar35) & ~(~(bVar18 & bVar35) & bVar18));
  if ((((bVar17 == 0x7e) && (bVar18 == 0x60)) && (bVar19 == 0x77)) &&
     (((bVar20 == 0x4c && (bVar21 == 0xd)) &&
      ((bVar22 == 0xc && ((bVar23 == 0x49 && (bVar24 == 0x4b)))))))) {
    if ((((byte)(~(~(bVar17 & bVar1) & bVar1) & ~(~(bVar17 & bVar1) & bVar17)) == 0xc2) &&
        (((((byte)(~(~(bVar18 & bVar2) & bVar2) & ~(~(bVar18 & bVar2) & bVar18)) == 0xcd &&
           ((byte)(~(~(bVar19 & bVar3) & bVar3) & ~(~(bVar19 & bVar3) & bVar19)) == 0xbb)) &&
          ((byte)(~(~(bVar20 & bVar4) & bVar4) & ~(~(bVar20 & bVar4) & bVar20)) == 200)) &&
         (((byte)(~(~(bVar21 & bVar5) & bVar5) & ~(~(bVar21 & bVar5) & bVar21)) == 0xb1 &&
          ((byte)(~(~(bVar22 & bVar6) & bVar6) & ~(~(bVar22 & bVar6) & bVar22)) == 0xa6)))))) &&
       ((byte)(~(~(~(bVar23 & bVar7) & bVar7) & ~(~(bVar23 & bVar7) & bVar23)) + 0x83 |
              ~(~(~(bVar24 & bVar8) & bVar8) & ~(~(bVar24 & bVar8) & bVar24))) == 0)) {
      if ((((byte)(~(~(bVar17 & bVar9) & bVar9) & ~(~(bVar17 & bVar9) & bVar17)) == 0xac) &&
          ((byte)(~(~(bVar18 & bVar10) & bVar10) & ~(~(bVar18 & bVar10) & bVar18)) == 0xad)) &&
         ((((byte)(~(~(bVar19 & bVar11) & bVar11) & ~(~(bVar19 & bVar11) & bVar19)) == 0xcc &&
           ((((byte)(~(~(bVar20 & bVar12) & bVar12) & ~(~(bVar20 & bVar12) & bVar20)) == 0x84 &&
             ((byte)(~(~(bVar21 & bVar13) & bVar13) & ~(~(bVar21 & bVar13) & bVar21)) == 0xbc)) &&
            ((byte)(~(~(bVar22 & bVar14) & bVar14) & ~(~(bVar22 & bVar14) & bVar22)) == 0xaa)))) &&
          (((byte)(~(~(bVar23 & bVar15) & bVar15) & ~(~(bVar23 & bVar15) & bVar23)) == 0xcb &&
           ((byte)(~(~(bVar24 & bVar16) & bVar16) & ~(~(bVar24 & bVar16) & bVar24)) == 0xb4)))))) {
        uVar33 = 3;
      }
      else {
        uVar33 = 2;
      }
    }
    else {
      uVar33 = 1;
    }
  }
  else {
    uVar33 = 0;
  }
  return uVar33;
}
```
3. Lift to z3
```py
# author: cpp.dog
from z3 import *

s = Solver()

a1 = Array("flag", IntSort(), BitVecSort(8))
a2 = Array("param1", IntSort(), BitVecSort(8))


"""
bVar17 = ~(~bVar1 & bVar9);
bVar18 = ~(~bVar2 & bVar10);
bVar19 = ~(~bVar3 & bVar11);
bVar20 = ~(~bVar4 & bVar12);
bVar21 = ~(~bVar5 & bVar13);
bVar22 = ~(~bVar6 & bVar14);
bVar23 = ~(~bVar7 & bVar15);
bVar24 = ~(~bVar8 & bVar16);
"""

bebra = [~(~a1[x] & a1[x + 8]) for x in range(8)]

"""
bVar17 = ~(~(bVar17 & bVar9) & ~(~bVar1 & bVar17));
bVar18 = ~(~(bVar18 & bVar10) & ~(~bVar2 & bVar18));
bVar19 = ~(~(bVar19 & bVar11) & ~(~bVar3 & bVar19));
bVar20 = ~(~(bVar20 & bVar12) & ~(~bVar4 & bVar20));
bVar21 = ~(~(bVar21 & bVar13) & ~(~bVar5 & bVar21));
bVar22 = ~(~(bVar22 & bVar14) & ~(~bVar6 & bVar22));
bVar23 = ~(~(bVar23 & bVar15) & ~(~bVar7 & bVar23));
bVar24 = ~(~(bVar24 & bVar16) & ~(~bVar8 & bVar24));
"""
for x in range(8):
  bebra[x] = ~(~(bebra[x] & a1[x + 8]) & ~(~a1[x] & bebra[x]))

"""
bVar25 = ~(bVar17 & ~(bVar1 | ~bVar9));
bVar26 = ~(bVar18 & ~(bVar2 | ~bVar10));
bVar27 = ~(bVar19 & ~(bVar3 | ~bVar11));
bVar28 = ~(bVar20 & ~(bVar4 | ~bVar12));
bVar29 = ~(bVar21 & ~(bVar5 | ~bVar13));
bVar30 = ~(bVar22 & ~(bVar6 | ~bVar14));
bVar31 = ~(bVar23 & ~(bVar7 | ~bVar15));
bVar32 = ~(bVar24 & ~(bVar8 | ~bVar16));
"""
bebra2 = [~(bebra[x] & ~(a1[x] | ~a1[x + 8])) for x in range(8)]

"""
  bVar17 = ~(~bVar9 | bVar1) | ~(bVar17 & bVar25) & ~(bVar25 & ~(bVar1 | ~bVar9));
  bVar18 = ~(~bVar10 | bVar2) | ~(bVar18 & bVar26) & ~(bVar26 & ~(bVar2 | ~bVar10));
  bVar19 = ~(~bVar11 | bVar3) | ~(bVar19 & bVar27) & ~(bVar27 & ~(bVar3 | ~bVar11));
  bVar20 = ~(~bVar12 | bVar4) | ~(bVar20 & bVar28) & ~(bVar28 & ~(bVar4 | ~bVar12));
  bVar21 = ~(~bVar13 | bVar5) | ~(bVar21 & bVar29) & ~(bVar29 & ~(bVar5 | ~bVar13));
  bVar22 = ~(~bVar14 | bVar6) | ~(bVar22 & bVar30) & ~(bVar30 & ~(bVar6 | ~bVar14));
  bVar23 = ~(~bVar15 | bVar7) | ~(bVar23 & bVar31) & ~(bVar31 & ~(bVar7 | ~bVar15));
  bVar24 = ~(~bVar16 | bVar8) | ~(bVar24 & bVar32) & ~(bVar32 & ~(bVar8 | ~bVar16));
"""
for x in range(8):
  bebra[x] = ~(~a1[x + 8] | a1[x]) | ~(bebra[x] & bebra2[x]) & ~(bebra2[x] & ~(a1[x] | ~a1[x + 8]))

x = 0
bebra[x] = ~(~(~(bebra[x] & a2[0]) & a2[0]) & ~(~(bebra[x] & a2[0]) & bebra[x]))
bebra[x + 1] = ~(~(~(bebra[x + 1] & a2[1]) & a2[1]) & ~(~(bebra[x + 1] & a2[1]) & bebra[x + 1]))

s.add(And(bebra[0] == 0x7e, bebra[1] == 0x60, bebra[2] == 0x77, bebra[3] == 0x4c, bebra[4] == 0xd, bebra[5] == 0xc, bebra[6] == 0x49, bebra[7] == 0x4b))

bebra3 = [(~(~(bebra[x] & a1[x]) & a1[x]) & ~(~(bebra[x] & a1[x]) & bebra[x])) for x in range(8)]
"""
if (((
  (~(~(bVar17 & bVar1) & bVar1) & ~(~(bVar17 & bVar1) & bVar17)) == 0xc2) &&
        ((((
  (~(~(bVar18 & bVar2) & bVar2) & ~(~(bVar18 & bVar2) & bVar18)) == 0xcd &&
           (
  (~(~(bVar19 & bVar3) & bVar3) & ~(~(bVar19 & bVar3) & bVar19)) == 0xbb)) &&
          (
  (~(~(bVar20 & bVar4) & bVar4) & ~(~(bVar20 & bVar4) & bVar20)) == 200)) &&
         ((
  (~(~(bVar21 & bVar5) & bVar5) & ~(~(bVar21 & bVar5) & bVar21)) == 0xb1 &&
          (
  (~(~(bVar22 & bVar6) & bVar6) & ~(~(bVar22 & bVar6) & bVar22)) == 0xa6)))))) &&
       ((
  ~(~(~(bVar23 & bVar7) & bVar7) & ~(~(bVar23 & bVar7) & bVar23)) + 0x83 |
  ~(~(~(bVar24 & bVar8) & bVar8) & ~(~(bVar24 & bVar8) & bVar24)) ) == 0)) {
"""
s.add(And(bebra3[0] == 0xc2, bebra3[1] == 0xcd, bebra3[2] == 0xbb, bebra3[3] == 200, bebra3[4] == 0xb1, bebra3[5] == 0xa6, (~bebra3[6] + 0x83 | ~bebra3[7]) == 0))

bebra3 = [(~(~(bebra[x] & a1[x + 8]) & a1[x + 8]) & ~(~(bebra[x] & a1[x + 8]) & bebra[x])) for x in range(8)]

s.add(And(bebra3[0] == 0xac, bebra3[1] == 0xad, bebra3[2] == 0xcc, bebra3[3] == 0x84, bebra3[4] == 0xbc, bebra3[5] == 0xaa, bebra3[6] == 0xcb, bebra3[7] == 0xb4))
"""
(~(~(bVar17 & bVar9) & bVar9) & ~(~(bVar17 & bVar9) & bVar17)) == 0xac) &&
(~(~(bVar18 & bVar10) & bVar10) & ~(~(bVar18 & bVar10) & bVar18)) == 0xad) &&
(~(~(bVar19 & bVar11) & bVar11) & ~(~(bVar19 & bVar11) & bVar19)) == 0xcc &&
(~(~(bVar20 & bVar12) & bVar12) & ~(~(bVar20 & bVar12) & bVar20)) == 0x84 &&
(~(~(bVar21 & bVar13) & bVar13) & ~(~(bVar21 & bVar13) & bVar21)) == 0xbc) &&
(~(~(bVar22 & bVar14) & bVar14) & ~(~(bVar22 & bVar14) & bVar22)) == 0xaa) &&
(~(~(bVar23 & bVar15) & bVar15) & ~(~(bVar23 & bVar15) & bVar23)) == 0xcb &&
(~(~(bVar24 & bVar16) & bVar16) & ~(~(bVar24 & bVar16) & bVar24)) == 0xb4)
"""

print(s.check())

m = s.model()
res = []
for i in range(34):
    res += (chr(int(str(m.evaluate(a1[i])))))
print("".join(res))
```
