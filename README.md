# CES : Camera Emulation Server 1.0.1

[![License](https://img.shields.io/badge/license-Apache-blue.svg)](#license)
[![Latest release](https://img.shields.io/badge/release-1.0.0-green.svg)](https://github.com/EtixLabs/CES/releases/latest)

### RTSP server with authentication for testing purposes

## Dependencies

* `docker`

## Usage from the official docker repository

You can create a stream by launching the official docker image: 
`docker run --rm -p 8554:8554 ullaakut/ces`

## Build and tweak yourself

You can tweak CES and create your own docker image. For this simply run: 
`./build.sh`

Then launch it with:
`docker run --rm -p 8554:8554 ces`

## Read the stream

With default options, stream will be available at `rtsp://0.0.0.0:8554/live.sdp`

## Override options

All of these environment variables override the default parameters for CES
* `RTSP_ADDRESS`: The address you want your server to listen on [default: `0.0.0.0`]
* `RTSP_PORT`: The port that you want your server to listen on [default: `8554`] _Don't forget to also expose the port in your container with the -p option like in the example above_
* `RTSP_ROUTE`: The rtsp route at which you want your stream to be served [default: `/live.sdp`]
* `RTSP_INPUT_FILE`: The video file you want to broadcast using CES [default: none]
* `RTSP_USERNAME`: If you want to enable security on your stream, using this option will allow you to specify the username required to access your stream [default: none]
* `RTSP_PASSWORD`: If you want to enable security on your stream, using this option will allow you to specify the password required to access your stream [default: none]
* `RTSP_RESOLUTION`: The resolution at which you want to stream [default: `352x288`]
* `RTSP_FRAMERATE`: The desired output framerate for your stream [default: `25`]
* `GST_DEBUG`: The desired debug level for GStreamer [default: none] _See [this link](https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/gst-running.html) for more information on this variable_

```
docker run \
       [-e RTSP_ADDRESS=your_address] \
       [-e RTSP_PORT=your_port] -p your_port:your_port \
       [-e RTSP_ROUTE=your_route] \
       [-e RTSP_INPUT_FILE=your_input_file] \
       [-e RTSP_USERNAME=your_username] \
       [-e RTSP_PASSWORD=your_password] \
       [-e RTSP_RESOLUTION='your_width'x'your_height'] \
       [-e RTSP_FRAMERATE=your_framerate] \
       [-e GST_DEBUG=your_debug_level] \
       ullaakut/CES
```

### Parameters

## License

Copyright 2017 Etix Labs

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.
