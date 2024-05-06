from os import environ
from sys import argv
from typing import Any

from uvicorn import run


DEV_MODE: bool = argv.count('--dev') > 0


def main() -> None:
    kw: dict[str, Any] = {
        'host': '0.0.0.0',
        'port': 1337,
        'workers': 5,
        'forwarded_allow_ips': '*',
        'proxy_headers': True,
    }

    if DEV_MODE or 'PyCharm' in environ.get('LIBRARY_ROOTS', ''):
        del kw['workers']
        del kw['proxy_headers']

    run(
        'shippering:app',
        **kw
    )


if __name__ == '__main__':
    main()
