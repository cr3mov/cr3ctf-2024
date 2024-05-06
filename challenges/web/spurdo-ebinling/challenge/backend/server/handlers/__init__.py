from importlib import import_module
from pathlib import Path

from loguru import logger

from ..core.handlers import registered_handlers


@logger.catch
def setup_handlers() -> int:
    path = Path(__file__).parent.resolve().absolute()

    count: int = 0
    for item in path.glob('*.py'):
        if item.stem == '__init__':
            continue

        logger.info(f'Loading {item.stem} handler')
        import_module(f'server.handlers.{item.stem}')
        count += 1

    return sum(len(x) for x in registered_handlers.values())
