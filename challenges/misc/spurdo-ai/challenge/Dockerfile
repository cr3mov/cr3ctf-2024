FROM ubuntu:20.04

RUN apt-get update \
    && apt-get install -yq --no-install-recommends  \
    python3-pip  \
    socat  \
    gcc make golang libgmp3-dev libc-dev \
    && rm -rf /var/lib/apt/lists/*

RUN python3 -m pip install requests emoji

COPY challenge.py /app/
COPY --from=es3n1n/pow-proxy:latest /app/main /app/pow

ENV LISTEN_PORT=1337 FORWARD_PORT=1338 POW_DIFFICULTY=5000 CONN_LIFETIME_MS=30000

CMD /app/pow \
    & socat \
      TCP-LISTEN:1338,reuseaddr,fork \
      EXEC:"python3 /app/challenge.py"
