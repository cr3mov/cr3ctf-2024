from z3 import *
import json
import random

delete = -578794842304260765 + 0x10000001

print(delete)

s = Solver()
cArr2 = Array("cArr2", IntSort(), BitVecSort(8))

meme = b"https://wrong-flutter-4e2b4c686579.1337.sb/login"
if True:
  meme2 = [ord(chr(x)) for x in meme]

  meme2 = [random.randint(16384, 45412), random.randint(16384, 45412), random.randint(16384, 45412), random.randint(16384, 45412)] + meme2

  i4 = 4
  while True:
    if i4 >= len(meme2):
      break
    meme2[i4] = (meme2[i4] ^ meme2[i4 % 4] ^ (i4 - 4) * delete) & 0xFFFF
    i4 += 1

  print(meme2)
  print(json.dumps("".join([chr(x) for x in meme2])))
else:
  meme2 = [random.randint(16384, 45412)]
  i4 = 1
  while True:
    if i4 > len(meme):
      break
    meme2.append(
      (meme[i4 - 1] ^ (i4 * meme2[0]) ^ delete) & 0xffff
    ) 
    i4 += 1

  print(meme2)
  print(len(meme))
  print(len(meme2))
  print(json.dumps("".join([chr(x) for x in meme2])))

print()
meme2 = "\u6e94\u3b8c\u8869\u1ebe\uef57\u7dea\uc272\u5301\u21e1\ub66d\u04cd\u955e\u7bff\uc894\u591e\u2fe3\ubc01\u02d4\u9363\u61fa\uf690\u472c\ud5a8\uba5a\u08ee\u9935\u6e51".encode()
print(meme2)
print(meme2[0])
meme = b'{"admin":false,"checksum":'
i4 = 1
while True:
  if i4 > len(meme):
    break
  if meme[i4 - 1] != (meme2[i4] ^ (i4 * meme2[0]) ^ delete) & 0xffff:
    print(f"i4 failed {i4}")
  i4 += 1
exit(0)


i4 = 1
while True:
  if i4 > len(meme):
    break
  s.add(meme[i4 - 1] == (cArr2[i4 - 1] ^ (i4 * cArr2[0]) ^ delete))
  i4 += 1
#s.add(enc == "com.google.android.material.button.MaterialButton")

if s.check() != sat:
  print("unsat")
  exit(0)

m = s.model()
res = []
for i in range(len(meme)):
    res += (chr(int(str(m.evaluate(cArr2[i])))))
print(b"".join(res))