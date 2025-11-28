# syntax=docker/dockerfile:1

## Build stage: compile Angular app
FROM node:20-slim AS build

WORKDIR /workspace
COPY . .
WORKDIR /workspace/heapmonj/frontend/heapmonj-frontend

RUN npm ci
RUN npm run build

## Runtime stage: serve with nginx and proxy /api to backend
FROM nginx:1.27-alpine

COPY heapmonj/frontend/nginx.conf /etc/nginx/conf.d/default.conf
COPY --from=build /workspace/heapmonj/frontend/heapmonj-frontend/dist/heapmonj-frontend/browser /usr/share/nginx/html

EXPOSE 80
