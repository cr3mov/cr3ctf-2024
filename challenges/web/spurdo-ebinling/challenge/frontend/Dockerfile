FROM nginx:alpine

#COPY /etc/apt/sources.list /etc/apt/sources.list

WORKDIR /app

COPY . .
RUN rm nginx.conf

COPY ./nginx.conf /etc/nginx/nginx.conf