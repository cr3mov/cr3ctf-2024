FROM node:latest

#COPY /etc/apt/sources.list /etc/apt/sources.list

WORKDIR /app

COPY package.json .
COPY index.js .

RUN npm install

CMD ["node", "index.js"]
