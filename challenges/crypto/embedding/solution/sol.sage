from sage.all import *
import numpy as np

ct = [1614.41597751+2440.04175266j, -239.31512831  +65.01569777j,
       -174.3244422  +623.0315122j ,  148.33319848 +107.54945904j,
        -80.39944861  -16.1430125j ,  106.05365816 +198.8020629j ,
        252.91493127  +79.94326544j, -102.92505223 +220.19525344j]

deg = 128
K = RealField(50)
zeta = e ^ (2 * pi * I / deg)

mat = []
scale = 2**25

for i in range(42):
    row = [0] * i + [1] + [0] * (42 - i - 1) 
    for j in range(len(ct)):
        x = (zeta ^ (2 * j + 1)) ^ (i)
        row.append(K(x.real()) * scale)
        row.append(K(x.imag()) * scale)
    mat.append(row)

row = [0] * 42
for j in range(len(ct)):
    row.append(-K(ct[j].real()) * scale)
    row.append(-K(ct[j].imag()) * scale)

mat.append(row)
mat = matrix(QQ,mat)
mat = mat.LLL()

for row in mat:
    # print(row)
    if all((j>0 and j<128) for j in row[:42]):
        flag = ''.join(chr(int(j)) for j in row[:42])
        print(flag)
       


