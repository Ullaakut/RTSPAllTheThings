# Etix RTSP server

[![cameradar License](https://img.shields.io/badge/license-Apache-blue.svg)](#license)

### RTSP server with authentication for testing purposes

### Dependencies

* cmake (`cmake`)
* glib-2.0
* [gstreamer 1.8 or later](https://github.com/GStreamer/gstreamer)

#### Step-by-step installation
1. `git clone ssh://git@git.etixlabs.com/cctv/etix_rtsp_server.git`
2. `cd etix_rtsp_server`
3. `mkdir build`
4. `cd build`
5. `cmake ..`
6. `make`
7. `./etix_rtsp_server [-r route] [-u username] [-p password]`
