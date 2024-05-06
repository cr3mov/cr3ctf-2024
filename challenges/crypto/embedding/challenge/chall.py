import numpy as np
from numpy.polynomial import Polynomial

class Encoder:
  def __init__(self, deg: int):
    self.zeta = np.exp(2 * np.pi * 1j / deg)
    self.poly_degree = deg

  def pi(self, z: np.array) -> np.array:
    N = self.poly_degree // 16
    return z[:N]
  
  def encode(self, p: Polynomial) -> np.array:
    z = self.sigma(p)
    pi_z = self.pi(z)
    return pi_z

  def sigma(self, p: Polynomial) -> np.array:
    N = self.poly_degree // 2
    outputs = [p(self.zeta ** (2*i + 1)) for i in range(N)]
    return np.array(outputs)

flag = "cr3{f4ke_fl4g}"
assert flag[:4]=='cr3{' and flag[-1] == '}'

flag = flag[4:-1].encode()
pt = list(flag)
assert len(pt) == 42, len(pt)
p = Polynomial(pt)

encoder = Encoder(128)
ct = encoder.encode(p)
print(f"{ct = }")

# ct = array([1614.41597751+2440.04175266j, -239.31512831  +65.01569777j,
#        -174.3244422  +623.0315122j ,  148.33319848 +107.54945904j,
#         -80.39944861  -16.1430125j ,  106.05365816 +198.8020629j ,
#         252.91493127  +79.94326544j, -102.92505223 +220.19525344j])