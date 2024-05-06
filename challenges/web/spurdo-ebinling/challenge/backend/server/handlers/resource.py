from functools import lru_cache
from urllib.parse import urlparse
from urllib.request import urlopen

from ..core.config import config
from ..core.handlers import MessageHandler
from ..core.session import Session
from ..enums.packet_type import EPacketType
from ..proto.protocol_pb2 import CRequestResource, SRequestResource


handler = MessageHandler(EPacketType.RESOURCE)


@lru_cache(maxsize=128)
def request_url(url: str) -> bytes:
    return urlopen(url).read()


DEFAULT = request_url(f'http://127.0.0.1:{config.CDN_PORT}/default.png')


@handler.dispatcher()
async def on_resource_request(session: Session, msg: CRequestResource) -> SRequestResource:
    result = SRequestResource(item=DEFAULT, success=False)

    try:
        msg_item = msg.item.split('?')[0]
        info = urlparse(msg_item)
    except Exception as e:  # noqa
        return result

    if info.scheme in ['file']:
        return result

    try:
        data = request_url(msg_item)
        if b'cr3{' in data:
            session.game.is_cheater = True
            session.log_warn('Cheater!')
        result.success = True
    except Exception:  # noqa
        pass

    return result
