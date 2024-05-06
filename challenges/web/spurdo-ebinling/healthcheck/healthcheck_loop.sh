#!/bin/bash
set -Eeuo pipefail

TIMEOUT=20
PERIOD=300
DELAY=15

export TERM=linux
export TERMINFO=/etc/terminfo

sleep "${DELAY}"

while true; do
  echo -n "[$(date)] "
  if timeout "${TIMEOUT}" /home/user/healthcheck.py; then
    echo 'ok' | tee /tmp/healthz
  else
    echo -n "$? "
    echo 'err' | tee /tmp/healthz
  fi
  sleep "${PERIOD}"
done