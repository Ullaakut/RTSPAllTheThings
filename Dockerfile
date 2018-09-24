FROM ubuntu:17.10 AS build

MAINTAINER brendan.le-glaunec@epitech.eu

RUN apt-get update && apt-get install --no-install-recommends -y \
    build-essential \
    libgstrtspserver-1.0-dev \
    git \
    ca-certificates \
    cmake &&\
    apt-get clean &&\
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

WORKDIR /build

COPY . .

RUN cmake . && make

FROM ubuntu:17.10

RUN apt-get update && apt-get install --no-install-recommends -y \
    libgstrtspserver-1.0-dev \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-good \
    gstreamer1.0-x \
    gstreamer1.0-libav \
    gstreamer1.0-tools && \
    apt-get clean &&\
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

COPY --from=build /build/rtspatt /

EXPOSE 8554

ENTRYPOINT ["/rtspatt"]
