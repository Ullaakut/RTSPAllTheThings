FROM ubuntu:16.04

MAINTAINER brendan.le-glaunec@epitech.eu

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

COPY rtspatt /
EXPOSE 8554

ENTRYPOINT ["/rtspatt"]
