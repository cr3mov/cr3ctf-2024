FROM sagemath/sagemath:latest as chroot

USER root
WORKDIR /app

ARG FLAG
RUN echo "$FLAG" > /flag

COPY . .
RUN chmod +x run

RUN sage --preparse ./main.sage

FROM pwn.red/jail:latest

COPY --from=chroot / /srv
ENV JAIL_MEM=256M JAIL_PIDS=30 JAIL_ENV_HOME=/tmp JAIL_TMP_SIZE=1M JAIL_CPU=1000 JAIL_POW=5000 JAIL_PORT=1337
ENV JAIL_TIME=40
