FROM ubuntu:16.10

RUN apt-get update && apt-get install --no-install-recommends -y \
    libgstrtspserver-1.0-dev \
    gstreamer1.0-plugins-ugly && \
    apt-get clean &&\
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ADD ces /
EXPOSE 8554
ENTRYPOINT ["/ces"]
