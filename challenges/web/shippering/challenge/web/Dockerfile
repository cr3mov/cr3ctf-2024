FROM python:3.11-buster as builder

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN pip install poetry==1.5.1

ENV POETRY_NO_INTERACTION=1 \
    POETRY_VIRTUALENVS_IN_PROJECT=1 \
    POETRY_VIRTUALENVS_CREATE=1 \
    POETRY_CACHE_DIR=/tmp/poetry_cache

RUN /usr/sbin/useradd -u 1000 user
WORKDIR /home/user/

COPY pyproject.toml poetry.lock ./

RUN poetry install --without dev --no-root && rm -rf $POETRY_CACHE_DIR

FROM python:3.11-slim-buster as chroot

RUN /usr/sbin/useradd -u 1000 user
WORKDIR /home/user

ENV VIRTUAL_ENV=/home/user/.venv \
    PATH="/home/user/.venv/bin:$PATH"

COPY --from=builder ${VIRTUAL_ENV} ${VIRTUAL_ENV}
COPY . .
COPY flag /

FROM python:3.11-slim-buster as runtime

WORKDIR /home/user/

RUN apt-get -y update && apt-get install -y \
    autoconf \
    bison \
    flex \
    gcc \
    g++ \
    git \
    libprotobuf-dev \
    libnl-route-3-dev \
    libtool \
    make \
    pkg-config \
    protobuf-compiler \
    && rm -rf /var/lib/apt/lists/* \
    && git clone https://github.com/google/nsjail.git /nsjail \
    && cd /nsjail \
    && make \
    && mv /nsjail/nsjail /usr/bin \
    && rm -rf /nsjail \
    && apt-get autoremove -y \
    && apt-get remove --purge -y  \
    autoconf \
    bison \
    flex \
    gcc \
    g++ \
    git \
    libprotobuf-dev \
    libnl-route-3-dev \
    libtool \
    make \
    pkg-config \
    protobuf-compiler \
    && rm -rf /var/lib/apt/lists/*

RUN /usr/sbin/useradd -u 1000 user \
    && chown -R user:user .
USER user

COPY --from=chroot / /chroot
ENTRYPOINT ["nsjail", "--config", "/chroot/home/user/nsjail.cfg", "--", "/home/user/.venv/bin/python3", "/home/user/main.py"]
