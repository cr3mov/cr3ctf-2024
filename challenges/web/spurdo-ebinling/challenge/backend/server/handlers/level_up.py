from ..const.numbers import xp_required_to_level_up
from ..core.handlers import MessageHandler
from ..core.session import Session
from ..enums.packet_type import EPacketType
from ..proto.protocol_pb2 import CLevelUp, SLevelUp


handler = MessageHandler(EPacketType.LEVEL_UP)
handler.set_reject_value(SLevelUp())


@handler.validator()
async def on_level_up_validation(session: Session, message: CLevelUp) -> bool:
    if message.item <= session.game.level:
        return False

    return True


@handler.dispatcher()
async def on_level_up(session: Session, message: CLevelUp) -> SLevelUp:
    while message.item > session.game.level:
        required = xp_required_to_level_up(session.game.level)

        if session.game.xp_count < required:
            break

        session.game.xp_count -= required
        session.game.level += 1

    session.log_info(f'Level upped to {session.game.level}')
    return SLevelUp()
