FROM python:3.11-slim

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

RUN useradd --create-home appuser
RUN apt-get update \
    && apt-get install -y --no-install-recommends make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

WORKDIR /app/pkixwebadm

RUN make vendor-libdmotservices

RUN pip install --no-cache-dir --upgrade pip \
    && pip install --no-cache-dir /app/libdmotservices/python \
    && pip install --no-cache-dir /app/pkixwebadm

USER appuser

WORKDIR /app/pkixwebadm

ENV PKIXWA_HOST=0.0.0.0 \
    PKIXWA_PORT=8080 \
    PKIXWA_DEV_MODE=false

EXPOSE 8080

CMD ["python", "pkixwebadm.py", "serve", "--host", "0.0.0.0", "--port", "8080"]
