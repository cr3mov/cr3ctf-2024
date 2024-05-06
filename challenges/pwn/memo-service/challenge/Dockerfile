FROM ubuntu:22.04 as chroot

RUN /usr/sbin/useradd --no-create-home -u 1000 user

COPY flag /app/
COPY chal /app/run

FROM pwn.red/jail

COPY --from=chroot / /srv
ENV JAIL_POW=0 JAIL_PORT=1337

WORKDIR /srv/app
RUN chmod +x run