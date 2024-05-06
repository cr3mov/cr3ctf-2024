from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from slowapi.errors import RateLimitExceeded
from starlette.requests import Request
from starlette.responses import JSONResponse, Response

from .routers.main import router as main_router
from .util.fs import ROOT_DIR


def _rate_limit_exceeded_handler(_: Request, exc: RateLimitExceeded) -> Response:
    return JSONResponse(
        {'error': f'Rate limit exceeded: {exc.detail}'}, status_code=429
    )


app = FastAPI(
    docs_url=None,
    redoc_url=None,
    openapi_url=None,
)
app.add_exception_handler(RateLimitExceeded, _rate_limit_exceeded_handler)  # type: ignore
app.mount('/static', StaticFiles(directory=ROOT_DIR / 'static'), name='static')
app.include_router(main_router)
