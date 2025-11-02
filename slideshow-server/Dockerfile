# syntax=docker/dockerfile:1

## Stage 1: build artifacts and run tests to verify correctness
FROM python:3.12-slim AS builder

ENV PYTHONUNBUFFERED=1

RUN apt-get update && apt-get install -y --no-install-recommends make && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app/slideshow-server
COPY slideshow-server/ ./

RUN pip install --no-cache-dir pytest && \
    make clean && \
    make test && \
    make dist


## Stage 2: runtime image with just the application and the required ffmpeg dependency
FROM python:3.12-slim

ENV PYTHONUNBUFFERED=1 \
    SLIDESHOW_MEDIA_DIR=/media

RUN apt-get update && apt-get install -y --no-install-recommends ffmpeg && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /opt/slideshow-server
COPY --from=builder /app/slideshow-server/ ./

RUN mkdir -p /media && \
    cp -R publicdomainpictures.net/. /media/

EXPOSE 8080

ENTRYPOINT ["python", "-m", "slideshow_server"]
CMD ["/media", "--bind_address", "0.0.0.0", "--port", "8080"]
