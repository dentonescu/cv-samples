# syntax=docker/dockerfile:1

## Stage 1: build Angular assets
FROM node:20-alpine AS build
WORKDIR /app

COPY wifiequ-frontend/package*.json ./
RUN npm ci
COPY wifiequ-frontend/ .
RUN npm run build -- --configuration production

## Stage 2: serve via nginx with key injection proxy
FROM nginx:1.27-alpine
WORKDIR /etc/nginx

RUN apk add --no-cache gettext \
    && mkdir -p /var/log/nginx /etc/nginx/logs

COPY nginx.conf /etc/nginx/nginx.conf.template
COPY docker/entrypoint-frontend.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

COPY --from=build /app/dist/wifiequ-frontend/browser /usr/share/nginx/html

ENV WFQ_BACKEND_URL=http://wifiequ:8080 \
    WFQ_FRONTEND_PORT=80 \
    WFQ_STATS_KEY=

EXPOSE 80

ENTRYPOINT ["/entrypoint.sh"]
