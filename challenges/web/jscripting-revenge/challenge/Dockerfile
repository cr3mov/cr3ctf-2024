FROM node:20.9.0 as chroot

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN /usr/sbin/useradd -u 1337 user
WORKDIR /home/user

COPY . .
RUN npm i && npm run build

FROM node:latest as runtime

WORKDIR /home/user/

RUN apt-get -y update && apt-get install -y \
    autoconf \
    bison \
    flex \
    gcc \
    g++ \
    git \
    libprotobuf-dev \
    libnl-route-3-dev \
    libtool \
    make \
    pkg-config \
    protobuf-compiler \
    && rm -rf /var/lib/apt/lists/* \
    && git clone https://github.com/google/nsjail.git /nsjail \
    && cd /nsjail \
    && make \
    && mv /nsjail/nsjail /usr/bin \
    && rm -rf /nsjail \
    && apt-get autoremove -y \
    && apt-get remove --purge -y  \
    autoconf \
    bison \
    flex \
    gcc \
    g++ \
    git \
    libprotobuf-dev \
    libnl-route-3-dev \
    libtool \
    make \
    pkg-config \
    protobuf-compiler \
    && rm -rf /var/lib/apt/lists/*

RUN /usr/sbin/useradd -u 1337 user \
    && chown -R user:user .
USER user

COPY --from=chroot / /chroot
ENTRYPOINT ["nsjail", "--config", "/chroot/home/user/nsjail.cfg", "--", "/usr/local/bin/node", "/home/user/app.js"]
