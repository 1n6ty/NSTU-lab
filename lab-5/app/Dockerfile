FROM node:latest

WORKDIR /app

COPY package*.json ./

RUN npm install

EXPOSE 80

CMD [ "node", "app.js" ]