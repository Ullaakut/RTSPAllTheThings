# CES : Camera Emulation Server 0.1.3

[![License](https://img.shields.io/badge/license-Apache-blue.svg)](#license)
[![Latest release](https://img.shields.io/badge/release-0.1.3-green.svg)](https://github.com/EtixLabs/CES/releases/latest)

### RTSP server with authentication for testing purposes

## Dependencies

* cmake (`cmake`)
* glib-2.0
* libgstrtspserver-1.0-dev
* [gstreamer 1.8 or later](https://github.com/GStreamer/gstreamer)

## Step-by-step installation
1. `git clone git@github.com:EtixLabs/CES.git`
2. `cd CES`
3. `mkdir build`
4. `cd build`
5. `cmake ..`
6. `make`
7. `Usage: ./camera_emulation_server [-l address] [-b port] [-r route] [-i input] [-u username] [-p password] [-s 'width'x'height'] [-f framerate]`

## Run in GStreamer debug mode

If you're experiencing problems with CES, know that you can run it while having GStreamer in debug mode by setting the value of your `GST_DEBUG` environment variable, depending on your needs. For example, `export GST_DEBUG="*:3"` should be a high enough level of debug for most problems you might encounter.

For more information on GST_DEBUG, check [this link](https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/gst-running.html)

## License

Copyright 2016 Etix Labs

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.
