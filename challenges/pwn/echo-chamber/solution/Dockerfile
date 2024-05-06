FROM ubuntu:22.04 as chroot

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y libc6:i386

RUN /usr/sbin/useradd --no-create-home -u 1000 user

COPY flag /
COPY chal /app/run

FROM pwn.red/jail

COPY --from=chroot / /srv
ENV JAIL_POW=0 JAIL_PORT=1337

WORKDIR /srv/app
RUN chmod +x run