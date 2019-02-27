FROM ubuntu:18.04 as builder

MAINTAINER brendan.le-glaunec@epitech.eu
RUN apt-get update && apt-get install --no-install-recommends -y \
    build-essential \
    libgstrtspserver-1.0-dev \
    cmake

COPY . /rtspatt-src
WORKDIR /rtspatt-build
RUN cmake ../rtspatt-src && make


FROM ubuntu:18.04

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

COPY --from=builder /rtspatt-build/rtspatt /rtspatt

ENTRYPOINT ["/rtspatt"]
