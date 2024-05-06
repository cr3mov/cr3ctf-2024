FROM ubuntu:22.04

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN apt-get update &&  \
    apt-get install -yq --no-install-recommends python3 python3-pip wget &&  \
    rm -rf /var/lib/apt/lists/*

RUN wget --no-check-certificate -O /tmp/go.tar.gz https://go.dev/dl/go1.22.1.linux-amd64.tar.gz
RUN tar -C /usr/local -xzf /tmp/go.tar.gz
ENV PATH="$PATH:/usr/local/go/bin"

RUN mkdir -p /app/

COPY . /app/

WORKDIR /app/
RUN go mod tidy && go mod verify
RUN go build -o chall ./main.go

CMD /app/chall