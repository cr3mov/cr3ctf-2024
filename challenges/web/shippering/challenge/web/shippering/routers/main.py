from sys import stderr
from traceback import extract_stack
from typing import Annotated

from fastapi import APIRouter, Form, HTTPException, Request
from fastapi.responses import HTMLResponse
from jinja2.sandbox import SandboxedEnvironment as Environment
from markovify import NewlineText  # type: ignore
from slowapi import Limiter
from slowapi.errors import RateLimitExceeded
from slowapi.util import get_remote_address

from ..util.fs import ROOT_DIR


templates = ROOT_DIR / 'templates'
resources = ROOT_DIR / 'resources'

template_index = (templates / 'index.html').read_text()
template_with_text = (templates / 'with_text.html').read_text()
model = NewlineText((resources / 'model.txt').read_text())

router = APIRouter()
limiter = Limiter(key_func=get_remote_address)

_isinstance = isinstance


def isinstance_hk(x, y):
    f = extract_stack(limit=2)
    # Fixing the stupid bugs by myself -_-
    if y is not list and len(f) > 0 and 'site-packages' in str(f[0].filename) and not any(
            x in str(f[0].filename) for x in [
                'fastapi',
                'pydantic',
                'typing',
                'click',
                'uvicorn',
                'h11',
                'starlette',
                'multipart',
                'slowapi',
                'limits',
                'lexer',
                'nodes',
                'visitor',
            ]
    ):
        return True
    return _isinstance(x, y)


isinstance_hk.__globals__['__builtins__']['isinstance'] = isinstance_hk


class Renderer:
    # Leftovers from the old version of renderer, do we even need this?
    @staticmethod
    def __sanitize(value: str | None) -> str:
        value = value or ''

        def throw_exc() -> None:
            raise HTTPException(status_code=422, detail='Nuh uh')

        try:
            value.encode().decode('ascii')
        except:  # noqa
            throw_exc()

        if any(x in value for x in [
            '_',
            '{{',
            '}}',
            '[',
            ']',
            '.',
            "'",
            ',',
        ]):
            throw_exc()

        return value

    @classmethod
    def __generate(cls, person_1: str | None, person_2: str | None) -> str:
        if not person_1 or not person_2:
            return ''

        generated_text: str = ''

        while '%PERSON1%' not in generated_text and '%PERSON2%' not in generated_text:
            for i in range(1):
                if i > 0:
                    generated_text += '\n'
                generated_text += model.make_sentence()

        return (generated_text
                .replace('%PERSON1%', cls.__sanitize(person_1))
                .replace('%PERSON2%', cls.__sanitize(person_2)))

    @staticmethod
    def __get_template(person_1: str | None, person_2: str | None) -> str:
        return template_index if not person_1 or not person_2 else template_with_text

    @classmethod
    async def stable(
            cls,
            person_1: str | None = None,
            person_2: str | None = None,
    ) -> str:
        return await Environment(enable_async=True).from_string(
            cls.__get_template(person_1, person_2)
        ).render_async(text_value=cls.__generate(person_1, person_2))

    # Old version, surely some day we should remove it
    @classmethod
    async def __old(
            cls,
            person_1: str | None = None,
            person_2: str | None = None,
    ) -> str:
        return await Environment(enable_async=True).from_string(
            cls
            .__get_template(person_1, person_2)
            .replace('{{text_value|e}}', cls.__generate(person_1, person_2))
        ).render_async(dir=dir)


renderer = Renderer()


async def render(
        person_1: str | None = None,
        person_2: str | None = None,
        method: str = 'stable'
) -> str:
    try:
        if method.startswith('__'):
            raise AttributeError()

        return await getattr(renderer, method)(person_1, person_2)
    except (AttributeError, SystemExit):
        return 'Access denied'


@router.get('/', response_class=HTMLResponse)
async def main_page():
    return await render()


@router.post('/', response_class=HTMLResponse)
@limiter.limit('1/second')
async def submitted_page(
        request: Request,
        person_1: Annotated[str | None, Form()] = None,
        person_2: Annotated[str | None, Form()] = None,
        method: Annotated[str, Form()] = 'stable',
):
    try:
        return await render(person_1, person_2, method)
    except Exception as e:
        if isinstance(e, HTTPException) or isinstance(e, RateLimitExceeded):
            raise e

        print(e, file=stderr)
        raise HTTPException(status_code=500, detail='what the hell men you broke my service')
