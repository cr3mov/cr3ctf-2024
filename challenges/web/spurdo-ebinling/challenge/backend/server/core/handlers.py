from collections import defaultdict
from typing import Any, Callable, Coroutine, Dict, List, Optional, TypeVar

from google.protobuf.message import Message as MessageProto
from loguru import logger

from ..enums.packet_type import EPacketType
from ..proto.protocol_pb2 import SPing
from .protocol import Message
from .session import Session


TMessageProto = TypeVar('TMessageProto', bound=MessageProto)
DispatcherType = Callable[[Session, TMessageProto], Coroutine[Any, Any, TMessageProto]]
ValidatorType = Callable[[Session, TMessageProto], Coroutine[bool, Any, TMessageProto]]
registered_handlers: Dict[EPacketType, List['MessageHandler']] = defaultdict(list)


class MessageHandler:
    def __init__(self, message_type: EPacketType) -> None:
        self.message_type = message_type
        self.validator_callback: Optional[ValidatorType] = None
        self.dispatcher_callback: Optional[DispatcherType] = None
        self.reject_value: Optional[TMessageProto] = None  # type: ignore
        self._subscribed: bool = False

    def _ensure_sub(self) -> None:
        if self._subscribed is True:
            return

        self._subscribed = True
        registered_handlers[self.message_type].append(self)

    def validator(self) -> Callable[[ValidatorType], ValidatorType]:
        def decorating_function(user_function: ValidatorType) -> ValidatorType:
            self.validator_callback = user_function
            self._ensure_sub()
            return user_function

        return decorating_function

    def dispatcher(self) -> Callable[[DispatcherType], DispatcherType]:
        def decorating_function(user_function: DispatcherType) -> DispatcherType:
            self.dispatcher_callback = user_function
            self._ensure_sub()
            return user_function

        return decorating_function

    def set_reject_value(self, value: TMessageProto) -> None:
        self.reject_value = value


@logger.catch
async def _call_validator(handler: MessageHandler, session: Session, data: TMessageProto) -> bool:
    if handler.validator_callback is None:
        return True
    return await handler.validator_callback(session, data)


async def _call_dispatcher(handler: MessageHandler, session: Session, data: TMessageProto) -> TMessageProto:
    assert handler.dispatcher_callback is not None
    return await handler.dispatcher_callback(session, data)


async def call_message_handlers(session: Session, message: Message, data: TMessageProto) -> TMessageProto:
    result: TMessageProto = SPing()  # type: ignore

    for handler in registered_handlers[message.packet_type]:
        if await _call_validator(handler, session, data) is False:
            assert handler.reject_value is not None
            result = handler.reject_value
            continue

        result = await _call_dispatcher(handler, session, data)

    return result
