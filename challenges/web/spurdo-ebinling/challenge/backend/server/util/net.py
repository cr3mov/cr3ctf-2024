from websockets import WebSocketServerProtocol


async def recv_str(ws_server: WebSocketServerProtocol) -> str:
    result = await ws_server.recv()
    if isinstance(result, bytes):
        result = result.decode('utf-8', errors='ignore')
    return result


async def recv_bytes(ws_server: WebSocketServerProtocol) -> bytes:
    result = await ws_server.recv()
    if isinstance(result, str):
        result = result.encode('utf-8')
    return result
