# CES : Camera Emulation Server 1.1.0

[![License](https://img.shields.io/badge/license-Apache-blue.svg)](#license)
[![Docker Pulls](https://img.shields.io/docker/pulls/ullaakut/ces.svg?style=flat)](https://hub.docker.com/r/ullaakut/ces/)
[![Latest release](https://img.shields.io/badge/release-1.1.0-green.svg)](https://github.com/EtixLabs/CES/releases/latest)

### RTSP server with authentication for testing purposes

## Dependencies

* `docker`

## Usage from the official docker repository

You can create a stream by launching the official docker image: 
`docker run --rm -p 8554:8554 ullaakut/ces`

With default options, stream will be available at `rtsp://0.0.0.0:8554/live.sdp`
You can use [override options](#override-options)

## Override options

```
docker run --rm \
       [-e RTSP_ADDRESS=your_address] \
       [-e RTSP_PORT=your_port] -p your_port:your_port \
       [-e RTSP_ROUTE=your_route] \
       [-e RTSP_USERNAME=your_username] \
       [-e RTSP_PASSWORD=your_password] \
       [-e RTSP_RESOLUTION='your_width'x'your_height'] \
       [-e RTSP_FRAMERATE=your_framerate] \
       [-e INPUT=your_input] \
       [-e GST_DEBUG=your_debug_level] \
       ullaakut/ces
```

All of these environment variables | command line arguments override the default parameters for CES
* `RTSP_ADDRESS` | -l : The address you want your server to listen on [default: `0.0.0.0`]
* `RTSP_PORT` | -b : The port that you want your server to listen on [default: `8554`] _Don't forget to also expose the port in your container with the -p option like in the example above_
* `RTSP_ROUTE` | -r : The rtsp route at which you want your stream to be served [default: `/live.sdp`]
* `RTSP_USERNAME` | -u : If you want to enable security on your stream, using this option will allow you to specify the username required to access your stream [default: none]
* `RTSP_PASSWORD` | -p : If you want to enable security on your stream, using this option will allow you to specify the password required to access your stream [default: none]
* `RTSP_RESOLUTION` | -s : The resolution at which you want to stream [default: `1280x720`] - CES will have to do encoding
* `RTSP_FRAMERATE` | -f : The desired output framerate for your stream [default: `25`] - CES will have to do encoding
* `INPUT` | -i : Input use as source. [default: videotestsrc with smtpe pattern] Parsed according to the syntax:
- "rtsp:" -> rtsp stream
- "pattern:" -> videotestsrc pattern _See [this link](https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-base-plugins/html/gst-plugins-base-plugins-videotestsrc.html#GstVideoTestSrcPattern) for more information on this variable_
- Non matching pattern -> file
* `GST_DEBUG`: The desired debug level for GStreamer [default: none] _See [this link](https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/gst-running.html) for more information on this variable_

## Contribute

You can modify CES and create your own docker image. For this simply run:
`./build.sh`

Then launch it with:
`docker run --rm -p 8554:8554 ces`

With default options, stream will be available at `rtsp://0.0.0.0:8554/live.sdp`
You can use [override options](#override-options)

## License

Copyright 2017 Etix Labs

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.
