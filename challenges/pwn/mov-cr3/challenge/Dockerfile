FROM ubuntu:22.04 as chroot

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN /usr/sbin/useradd --no-create-home -u 1000 user

RUN apt-get update \
    && apt-get install -yq qemu-system \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir /app 
COPY bzImage /app/
COPY rootfs.cpio /app/
COPY run.sh /app/run
RUN chmod 755 /app/run

FROM pwn.red/jail

COPY --from=chroot / /srv
ENV JAIL_POW=7500 JAIL_PORT=1337 JAIL_CPU=1000 JAIL_MEM=200M JAIL_TIME=300
