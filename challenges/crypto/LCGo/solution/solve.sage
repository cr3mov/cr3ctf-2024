from Crypto.Util.number import long_to_bytes

n = next_prime(2^1024)

res = list(map(lambda x: int(x), open("out.txt", "r").read().strip().split("\n")))
assert len(res) == 16
u = len(res) - 1

A = [u for u in res[:-1]]
B = [-u for u in res[1:]]

K = 2^768
X = n * identity_matrix(QQ, u)
Z = matrix(QQ, [0] * u + [K / n] + [0]).transpose()
Z2 = matrix(QQ, [0] * (u + 1) + [K]).transpose()

Y = block_matrix([[X],[matrix(QQ, A)], [matrix(QQ, B)]])
Y = block_matrix([[Y, Z, Z2]])

Y = Y.LLL()

flag = ZZ(Y[1, -2] / (K / n) % n)
print(long_to_bytes(flag).decode()) # cr3{LFG_b4byyy_LLLCGO}
