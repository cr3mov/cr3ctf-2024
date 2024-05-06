FROM python:3.10.0-buster as builder

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN pip install poetry==1.8.1

ENV POETRY_NO_INTERACTION=1 \
    POETRY_VIRTUALENVS_IN_PROJECT=1 \
    POETRY_VIRTUALENVS_CREATE=1 \
    POETRY_CACHE_DIR=/tmp/poetry_cache

RUN /usr/sbin/useradd -u 1000 user
WORKDIR /home/user/

COPY pyproject.toml poetry.lock ./

RUN poetry install --without dev && rm -rf $POETRY_CACHE_DIR

FROM python:3.10.0-slim-buster as chroot

RUN /usr/sbin/useradd -u 1000 user
WORKDIR /home/user

ENV VIRTUAL_ENV=/home/user/.venv \
    PATH="/home/user/.venv/bin:$PATH"

COPY --from=builder ${VIRTUAL_ENV} ${VIRTUAL_ENV}
COPY . .

CMD ["python3", "main.py"]
