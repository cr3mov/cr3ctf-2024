FROM node:21 as chroot

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN apt-get update \
    && apt-get install -y \
    gnupg2 \
    wget \
    && wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | apt-key add - \
    && sh -c 'echo "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google.list' \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -yq --no-install-recommends \
        libasound2 \
        libatk-bridge2.0-0 \
        libatk1.0-0 \
        libcups2 \
        libgbm1 \
        libnss3 \
        libxcomposite1 \
        libxdamage1 \
        libxfixes3 \
        libxkbcommon-x11-0 \
        libxrandr2 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

ENV PUPPETEER_CACHE_DIR="/app/.cache/puppeteer"
RUN npm install puppeteer

# In seconds
ENV BOT_TIMEOUT="15"

RUN mkdir /tmp/chrome-userdata && chmod o+rwx /tmp/chrome-userdata

COPY cookie .
COPY bot.js .

CMD node /app/bot.js
