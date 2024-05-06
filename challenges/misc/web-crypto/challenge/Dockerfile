FROM ubuntu:20.04

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN apt-get update \
    && apt-get install -yq --no-install-recommends python3 python3-pip wget \
    && rm -rf /var/lib/apt/lists/*

RUN wget -O /tmp/go.tar.gz https://go.dev/dl/go1.21.3.linux-amd64.tar.gz
RUN tar -C /usr/local -xzf /tmp/go.tar.gz
ENV PATH="$PATH:/usr/local/go/bin"

RUN mkdir -p /app/

COPY go.mod /app/
COPY cmd /app/cmd/
COPY collection /app/collection/
COPY encryption /app/encryption/
COPY server /app/server/
COPY templates /app/templates/

WORKDIR /app

# proxy because for some fucking reason it returns 403 errors on the production environment??
ENV GOPROXY=https://goproxy.io,direct
RUN go mod tidy && go mod verify
RUN go build -o run ./cmd/main.go

CMD /app/run
