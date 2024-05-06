from websockets import WebSocketServerProtocol

from ..util.net import recv_bytes
from .handlers import call_message_handlers
from .messages import load_message
from .protocol import assemble_message_with_header, read_message
from .serializer import serialize_message
from .session import Session


async def receive_and_dispatch(session: Session, ws_server: WebSocketServerProtocol) -> None:
    session.clear_injected_packets()

    message = read_message(session, await recv_bytes(ws_server))
    deserialized_message = load_message(session, message)

    response = await call_message_handlers(session, message, deserialized_message)

    for item in [response, *session.injected_packets]:
        await ws_server.send(assemble_message_with_header(message.packet_type, serialize_message(item)))
