FROM node:20-bookworm AS builder

WORKDIR /app

COPY . .

RUN apt-get update \
    && apt-get install -y --no-install-recommends python3 \
    && rm -rf /var/lib/apt/lists/*

RUN python3 precompute/precompute.py --export-json tpg-spa/src/assets/states.json

WORKDIR /app/tpg-spa

RUN npm ci && npm run build

FROM nginx:1.27-alpine

COPY --from=builder /app/tpg-spa/dist /usr/share/nginx/html
