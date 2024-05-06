from ..core.handlers import MessageHandler
from ..core.session import Session
from ..enums.packet_type import EPacketType
from ..proto.protocol_pb2 import CPing, SPing


handler = MessageHandler(EPacketType.PING)


@handler.dispatcher()
async def on_ping(session: Session, _: CPing) -> SPing:
    session.log_info('Pong!')
    return SPing()
