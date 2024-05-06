from dataclasses import dataclass, field
from typing import Callable, NoReturn

from google.protobuf.message import Message as ProtoMessage
from loguru import logger
from websockets import WebSocketServerProtocol

from ..exceptions.drop_connection import DropConnectionException
from ..game.state import GameState


@dataclass
class Session:
    ip_address: str

    game: GameState = field(default_factory=GameState)

    injected_packets: list[ProtoMessage] = field(default_factory=list)

    @logger.catch
    def _aligned_log_prefix(self) -> str:
        return f'{self.ip_address:>15}: {self.game.wave_number:<3}: '

    @logger.catch
    def _log_stub(self, fn: Callable, message: str) -> None:
        fn(f'{self._aligned_log_prefix()}{message}')

    @logger.catch
    def log_info(self, message: str) -> None:
        self._log_stub(logger.info, message)

    @logger.catch
    def log_warn(self, message: str) -> None:
        self._log_stub(logger.warning, message)

    @logger.catch
    def log_error(self, message: str) -> None:
        self._log_stub(logger.error, message)

    def log_critical(self, message: str) -> NoReturn:
        self._log_stub(logger.critical, message)
        raise DropConnectionException()

    @logger.catch
    def inject_packet(self, message: ProtoMessage) -> None:
        self.injected_packets.append(message)

    @logger.catch
    def clear_injected_packets(self) -> None:
        self.injected_packets.clear()


@logger.catch
async def startup_session(ws_server: WebSocketServerProtocol) -> Session:
    session = Session(
        ip_address=ws_server.request_headers.get('X-Real-IP', ws_server.remote_address[0]),
    )

    session.log_info('Connected')
    return session
