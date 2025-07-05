# Stage 1: build
FROM ubuntu:22.04 AS builder
WORKDIR /app

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
      ca-certificates build-essential git libboost-all-dev libssl-dev zlib1g-dev libasio-dev wget ccache \
 && update-ca-certificates \
 && rm -rf /var/lib/apt/lists/*

RUN wget -O /tmp/crow.deb \
      https://github.com/CrowCpp/Crow/releases/download/v1.2.1.2/Crow-1.2.1-Linux.deb \
 && dpkg -i /tmp/crow.deb || (apt-get update && apt-get install -f -y) \
 && rm /tmp/crow.deb

COPY . .
RUN MODE=release make api

# Stage 2: runtime
FROM ubuntu:22.04
WORKDIR /app

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
      ca-certificates libssl-dev libboost-all-dev zlib1g libasio-dev \
 && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/api ./api

EXPOSE 443
CMD ["./api"]
