# Misc - W in web stands for crypto

### Description
Everyone knows W in web stands for crypto, so I made this totally secure secret collection. 

### Examining source code
This part is interesting.

```go
s.debugCookie = "$2a$04$5FGCCcPg1sELCWwnSqWYZemUYOqNIpjwo0l2KAw.1jmP0jLTEqcg."
...
debug, _ := c.Cookie("DEBUG")
err := bcrypt.CompareHashAndPassword(s.debugCookie, []byte(fmt.Sprintf("DEBUG_%s_DEBUG", debug)))
isDev := err == nil
```

Crack the debug cookie value:
`hashcat -m 3200 -a 0 '$2a$04$5FGCCcPg1sELCWwnSqWYZemUYOqNIpjwo0l2KAw.1jmP0jLTEqcg.' -j '^_^G^U^B^E^D$_$D$E$B$U$G' rockyou.txt`

Password is `camchito`, set this as cookie value.

Golang ranges before version 1.22 work not as you may think they should, so your iterable value has persistent address. We can find this out by placing some nasty logs in the challenge code.

https://github.com/golang/go/issues/22791#issuecomment-345391395

```go
for _, bi := range ba {

  // Avoid copying to speed this up, cuz algorithm takes a lot of time as security pay off 


  //   Bug is here
  //       |
  //       V
  res[i] = &bi
  i++

  if i == keyLength*256{
    break
  }
}
```

Cleaner code:

```go
for i, bi := range ba {
  res[i] = &bi
}
```

So, in order to decrypt the data you only need to brute 2 bytes, 1 for IV and 1 for key. Now we only need to request all the possible collection entries and look for flag. (It was hidden entry with key `flag`)

```py
import base64
import requests

from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

secrets = [
  "fanfic", "very-secret-totaly-not-flag", "secret",
  "gnu-linux", "recipe", "flag"
]

res = requests.post("http://challenge-url/collection", data={"secret":"flag"}, cookies={
  "DEBUG": "camchito"
})
DATA = base64.b64decode(res.json()['data'])

for c in range(256):
  for c1 in range(256):
    cipher = AES.new(c.to_bytes(1, 'little') * 32, AES.MODE_CBC, iv=c1.to_bytes(1, 'little') * 16)
    d = cipher.decrypt(DATA)
    try:
      data = unpad(d, 16)
      # if b"https" in data:
      if b"cr3" in data:
        print(data)
    except Exception as e:
      pass
```

### Flag
`cr3{s0o_uhHh_1_w4nna_s4y_h3l10_t0_my_m0m_s1st3rs_g1rlfr13nd(j0k3_1_d0nt_have_4ny)_d0g_c4t_p3t_c0ckr04ch_and_y0u_th3_p3rs0n_wh0_s0lv3d_th15_ch4l1eng3}`
