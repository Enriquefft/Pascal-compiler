FROM ubuntu:22.04

WORKDIR /app

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        git \
        cmake \
        libboost-all-dev \
        libssl-dev \
        zlib1g-dev \
        libasio-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Crow (header-only library)
RUN git clone https://github.com/CrowCpp/Crow.git /tmp/crow \
    && cp -r /tmp/crow/include/crow /usr/local/include/ \
    && rm -rf /tmp/crow

COPY . .

RUN make api

EXPOSE 443

CMD ["./build/api"]
