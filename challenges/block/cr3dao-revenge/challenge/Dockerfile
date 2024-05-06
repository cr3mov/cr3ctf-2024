FROM ghcr.io/foundry-rs/foundry:latest AS foundry

COPY project /project

RUN true && \
    cd /project && \
    forge build --out /artifacts/out --cache-path /artifacts/cache && \
    true

FROM es3n1n/paradigmctf.py:latest

COPY --chown=user:user . /challenge
COPY --from=foundry --chown=user:user /artifacts /artifacts

RUN chmod +x /challenge/challenge.py
COPY --from=es3n1n/pow-proxy:latest /app/main /app/pow

ENV LISTEN_PORT=1337 FORWARD_PORT=1338 POW_DIFFICULTY=0 CONN_LIFETIME_MS=30000

CMD /app/pow \
    & socat TCP-LISTEN:1338,reuseaddr,fork exec:"/challenge/challenge.py"
