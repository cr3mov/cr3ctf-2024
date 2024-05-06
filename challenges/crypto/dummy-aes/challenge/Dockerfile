FROM sagemath/sagemath:latest as chroot

USER root
WORKDIR /app

ARG FLAG
RUN echo "$FLAG" > /flag

RUN sage --pip install pycryptodome

COPY run run
RUN chmod +x run

#COPY aes.sage aes.sage
#RUN sage --preparse ./aes.sage
#RUN cp ./aes.sage.py ./aes.py
#
#COPY main.sage main.sage
#RUN sage --preparse ./main.sage

COPY aes.py aes.py
COPY main.py main.py

FROM pwn.red/jail:latest

COPY --from=chroot / /srv
ENV JAIL_MEM=256M JAIL_PIDS=30 JAIL_ENV_HOME=/tmp JAIL_TMP_SIZE=1M JAIL_CPU=1000 JAIL_POW=0 JAIL_PORT=1337
ENV JAIL_TIME=30
