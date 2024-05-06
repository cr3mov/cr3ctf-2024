#!/bin/bash
set -Eeuo pipefail

TIMEOUT=40
PERIOD=900

export TERM=linux
export TERMINFO=/etc/terminfo

while true; do
  rm -f /out
  echo -n "[$(date)] "
  if timeout "${TIMEOUT}" /home/user/invoke.sh; then
    if [ -s /out ]; then
      exit_code=$(cat /out)
      if [ "$exit_code" -eq 0 ]; then
        echo 'ok' | tee /tmp/healthz
      else
        echo "$exit_code "
        echo "err" | tee /tmp/healthz
      fi
    else
      echo -n "$? "
      echo 'err' | tee /tmp/healthz
    fi
  else
    echo -n "$? "
    echo 'err' | tee /tmp/healthz
  fi
  sleep "${PERIOD}"
done