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

CMD socat TCP-LISTEN:1337,reuseaddr,fork exec:"/challenge/challenge.py"
