# Getting Closer Damn

<html>
  <body>
    <table style="border: 1px solid grey; text-align: center">
      <tbody>
        <tr>
          <td style="border: 1px solid grey; text-align: center"><b>Category</b></td>
          <td style="border: 1px solid grey; text-align: center">Crypto</td>
        </tr>
        <tr>
          <td style="border: 1px solid grey; text-align: center"><b>Difficulty</b></td>
          <td style="border: 1px solid grey; text-align: center; color: 'green'">Very Easy</td>
        </tr>
        <tr>
          <td style="border: 1px solid grey; text-align: center"><b>Year</b></td>
          <td style="border: 1px solid grey; text-align: center">2024</td>
        </tr>
        <tr>
          <td style="border: 1px solid grey; text-align: center"><b>Author</b></td>
          <td style="border: 1px solid grey; text-align: center">r4sti</td>
        </tr>
      </tbody>
    </table>
  </body>
</html>

## Description

What is the correct one? Old but Gold, or Gold but Old?

## Synopsis

To solve the challenge, players have to calculate $gcd(N^a - 1, N^b - 1)$. However, trying to calculate $N^a$ and $N^b$ would take a very long time because the result is enormous. It turns out that the player can perform an equivalent computation to get the exact same value, namely $N^{gcd(a,b)} -1$. Due to the size of $a,b$, their gcd can be bruteforced.

## Overview

The source code is short and straight forward, as expected for an easy challenge. Let's describe the basic flow with the following steps:

- A value $N$ is generated which is a random 512-bit prime number.
- A pool of ten 9-bit primes is generated.
- $a$ and $b$ are computed as the product of three of these 9-bit primes.
- The sha256 hash digest of $gcd(N^a-1, N^b-1)$ becomes the AES encryption key.

The comment `pros of having a quantum computer ^-^` indicates that the computation of the gcd is very expensive, in terms of computational resources, and is probably intended for quantum computers. Therefore we have to find a different way to compute this value.

At this point it becomes obvious that the output file was not produced by the script as it is but an optimized formula was used.

## Solution

From a quick [search](https://math.stackexchange.com/questions/7473/prove-that-gcdan-1-am-1-a-gcdn-m-1) we can find out the optimized formula. Computing $gcd(N^a-1, N^b-1)$ is equivalent to computing $N^{gcd(a,b)}-1$. This is a basic theorem in number theory and its proof is omitted here for the sake of brevity.

However we are not done yet:

- We do not know neither $a$ nor $b$.
- How do we assume that $N^{gcd(a,b)}$ is computationally feasible to calculate?

Let's inspect how $a$ and $b$ are generated.

$$a = p_1p_2p_3\\b = q_1q_2q_3$$

where $p_i$ and $q_i$ are 9-bit primes. This means that $a$ and $b$ are at most $3 \cdot 9 = 27$ bits.

The pool of $p_i$ and $q_i$ is the same and is quite small so it is very likely that some $p_i$ is the same as some $q_j$ for $i,j \in [1,3]$. In that case, the gcd should be at least a $9$-bit number. Therefore all we have to do is bruteforce the gcd which should lie in $[2, 2^9)$ and try the ciphertext until the plaintext contains the flag format.

This idea is implemented in the following python code. By running the solver, we eventually get the flag.

```python
with open('output.txt') as f:
    N = int(f.readline().split(' = ')[1])
    out = eval(f.readline().split(' = ')[1])

iv = bytes.fromhex(out['iv'])
ct = bytes.fromhex(out['ct'])

for G in range(2**9):
    g = N**G - 1
    key = sha256(str(g).encode()).digest()[:16]
    cipher = AES.new(key, AES.MODE_CBC, iv)
    m = cipher.decrypt(ct)
    if b'cr3{' in m:
        print(unpad(m, 16).decode())
        break
```