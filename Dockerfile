FROM ubuntu:16.10

MAINTAINER brendan.leglaunec@etixgroup.com

RUN apt-get update && apt-get install --no-install-recommends -y \
    libgstrtspserver-1.0-dev \
    gstreamer1.0-plugins-ugly && \
    apt-get clean &&\
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

COPY rtspatt /
EXPOSE 8554
ENTRYPOINT ["/rtspatt"]
