from z3 import *

s = Solver()
x = BitVec('x', 64)
y = BitVec('y', 64)

# s.add(x == int.from_bytes(b'CR3{CU4K', 'little'))
# s.add(y == int.from_bytes(b'-2D7NY}', 'little'))

expr = (x ^ y) ^ 16
expr2 = expr ^ x
expr3 = expr ^ y

s.add(expr == 0x4b490c0d4c77607e)
s.add(expr2 == 0x7d594e3744323d)
s.add(expr3 == 0x4b3455437b335253)

s.check()
mdl = s.model()

print()
print('expr', hex(mdl.eval(expr).as_long()))
print('expr2', hex(mdl.eval(expr2).as_long()))
print('expr3', hex(mdl.eval(expr3).as_long()))

print()
print('x', hex(mdl[x].as_long()))
print('y', hex(mdl[y].as_long()))

print()
print(int.to_bytes(mdl[x].as_long(), 8, 'little').decode(), end='')
print(int.to_bytes(mdl[y].as_long(), 8, 'little').decode())
