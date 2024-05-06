FROM ubuntu:20.04 as chroot

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN apt-get update \
    && apt-get install -yq --no-install-recommends python3-pip \
    && rm -rf /var/lib/apt/lists/*

COPY flag /

FROM pwn.red/jail

COPY --from=chroot / /srv
COPY blacklist.py /srv/app/
COPY challenge.py /srv/app/run
RUN chmod +x /srv/app/run
RUN ln -s /flag /srv/app/flag

ENV JAIL_MEM=32M JAIL_POW=1 JAIL_PORT=1337
