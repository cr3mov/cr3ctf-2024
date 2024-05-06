FROM ubuntu:22.04

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN apt-get update -y &&  \
    apt-get install -yq --no-install-recommends python3 python3-pip curl gnupg wget &&  \
    curl --location --silent https://dl-ssl.google.com/linux/linux_signing_key.pub | apt-key add - &&  \
    sh -c 'echo "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google.list' && \
    apt-get update &&  \
    apt-get install google-chrome-stable -y --no-install-recommends &&  \
    rm -rf /var/lib/apt/lists/*

RUN wget -O /tmp/go.tar.gz https://go.dev/dl/go1.22.1.linux-amd64.tar.gz
RUN tar -C /usr/local -xzf /tmp/go.tar.gz
ENV PATH="$PATH:/usr/local/go/bin"

RUN mkdir /app

COPY . /app/

WORKDIR /app/
RUN go mod tidy && go mod verify
RUN go build -o chall ./cmd/main.go

CMD /app/chall --rod="bin=/usr/bin/google-chrome-stable"