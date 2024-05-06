from pydantic_settings import BaseSettings, SettingsConfigDict

from ..util.fs import ROOT_DIR


class Settings(BaseSettings):
    model_config = SettingsConfigDict(
        env_file=ROOT_DIR / '.env',
        env_file_encoding='utf-8'
    )

    HOST: str = '0.0.0.0'
    PORT: int = 13387
    CDN_PORT: int = 13388

    FLAG: str = 'cr3{fake_flag_congrats}'
    ALLOWED_ORIGINS: list[str] = [
        'http://localhost',
        'http://127.0.0.1'
    ]


config = Settings()  # type: ignore
