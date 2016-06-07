CES : Camera Emulation Server

[![cameradar License](https://img.shields.io/badge/license-Apache-blue.svg)](#license)
[![Latest release](https://img.shields.io/badge/release-0.1.0-green.svg)](https://github.com/EtixLabs/CES/releases/latest)

### RTSP server with authentication for testing purposes

## Dependencies

* cmake (`cmake`)
* glib-2.0
* [gstreamer 1.8 or later](https://github.com/GStreamer/gstreamer)

## Step-by-step installation
1. `git clone ssh://git@git.etixlabs.com/cctv/etix_rtsp_server.git`
2. `cd etix_rtsp_server`
3. `mkdir build`
4. `cd build`
5. `cmake ..`
6. `make`
7. `./etix_rtsp_server [-r route] [-u username] [-p password]`

## License

Copyright 2016 Etix Labs

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.
