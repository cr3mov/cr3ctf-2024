FROM ubuntu:20.04

#COPY /etc/apt/sources.list /etc/apt/sources.list

RUN apt-get update -y && apt-get install -y python3 python3-pip wget

RUN python3 -m pip install numpy flask waitress

RUN mkdir -p /home/user/
COPY main.py /home/user/
WORKDIR /home/user/

CMD python3 -m waitress --host 0.0.0.0 --port 1337 --no-expose-tracebacks "main:app"
