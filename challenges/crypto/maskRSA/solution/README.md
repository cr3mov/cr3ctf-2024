# maskRSA

* Difficulty: Easy - Medium

## Description

Can you solve this easy RSA challenge?

## Solution

The idea is to find the value of ${flag1}^{17}$ and ${flag2}^{17}$, then calculate the $17^{th}$-root to get the flag. We can achieve this in two ways:

* The first solution is to use **Gröbner Basis**. We only need 3 encryptions to compute the basis.
* The second solution is to use **Gauss Elimination** to calculate each coefficient of the encryption polynomials, since the challenge give us 20 encryptions while there are only 18 coefficients in each polynomial.

**Solve script:**

```python
from Crypto.Util.number import long_to_bytes, inverse
from sage.all import *
import gmpy2
from data import *

x,y = PolynomialRing(Zmod(n), ['x','y']).gens()
f = []

for i in range(len(masks)):
    f.append((masks[i]*x + (masks[i]+1)*y)**17 - enc[i])

mat = []
for j in range(20):
    row=[]
    for i in range(18):
        row.append(f[j].coefficient({x:i,y:17-i}))
    row.append(f[j].coefficient({x:0,y:0}))
    mat.append(row)

mat = matrix(ZZ,mat)
mat = mat.echelon_form()

# check the matrix
# for i in mat:
#     print(i)

# compute flag1**17 and flag1
a = -mat[0][-1] % n
print(long_to_bytes(gmpy2.iroot(a,17)[0]))

# compute flag2**17 and flag2
b = -mat[-3][-1] % n
print(long_to_bytes(gmpy2.iroot(b,17)[0]))

# flag = cr3{m4skR5a_1s_e4sy_r1ght??}
```
