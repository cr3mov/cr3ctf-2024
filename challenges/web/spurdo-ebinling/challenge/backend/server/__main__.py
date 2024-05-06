import contextlib
from asyncio import Future, run
from http.server import test  # type: ignore
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from socket import socket
from threading import Thread
from time import sleep

from loguru import logger
from websockets import WebSocketException, WebSocketServerProtocol
from websockets.server import serve

from .core.config import config
from .core.dispatcher import receive_and_dispatch
from .core.session import startup_session
from .exceptions.drop_connection import DropConnectionException
from .handlers import setup_handlers
from .util.fs import CDN_DIR


@logger.catch
async def serve_connection(websocket: WebSocketServerProtocol) -> None:
    origin: str = websocket.request_headers.get('Origin', 'http://localhost:13371')

    try:
        if any(origin.startswith(x) for x in config.ALLOWED_ORIGINS) is False:
            await websocket.send('Unsupported origin :^)')
            return

        session = await startup_session(websocket)

        while True:
            await receive_and_dispatch(session, websocket)
    except (WebSocketException, DropConnectionException):
        return


# ensure dual-stack is not disabled; ref #38907
class DualStackServer(ThreadingHTTPServer):
    def server_bind(self):
        # suppress exception when protocol is IPv4
        with contextlib.suppress(Exception):
            self.socket.setsockopt(
                socket.IPPROTO_IPV6, socket.IPV6_V6ONLY, 0)
        return super().server_bind()

    def finish_request(self, request, client_address):
        self.RequestHandlerClass(request, client_address, self, directory=str(CDN_DIR.absolute()))


def serve_cdn() -> None:
    test(
        HandlerClass=SimpleHTTPRequestHandler,
        ServerClass=DualStackServer,
        port=config.CDN_PORT,
        bind=config.HOST,
        protocol='HTTP/1.0',
    )


@logger.catch
async def async_main() -> None:
    logger.info('Starting CDN')
    Thread(target=serve_cdn).start()

    # No condition for yall
    sleep(5)

    logger.info(f'Initialized {setup_handlers()} handlers')
    logger.info(f'Allowed origins: {config.ALLOWED_ORIGINS}')

    async with serve(serve_connection, host=config.HOST, port=config.PORT, server_header='spurdo-leveling'):
        logger.info(f'Listening at {config.HOST}:{config.PORT}')
        await Future()


def main() -> None:
    try:
        run(async_main())
    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    main()
