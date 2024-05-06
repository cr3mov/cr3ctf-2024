FROM ubuntu:22.04@sha256:e6173d4dc55e76b87c4af8db8821b1feae4146dd47341e4d431118c7dd060a74 as chroot

RUN /usr/sbin/useradd --no-create-home -u 1000 user

COPY flag.txt /
COPY randomness_revenge /app/run
RUN chmod 444 /flag.txt
RUN chmod 755 /app/run

FROM pwn.red/jail

COPY --from=chroot / /srv
ENV JAIL_POW=0 JAIL_PORT=1337
