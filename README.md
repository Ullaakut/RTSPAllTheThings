# CES : Camera Emulation Server 1.0.0

[![License](https://img.shields.io/badge/license-Apache-blue.svg)](#license)
[![Latest release](https://img.shields.io/badge/release-1.0.0-green.svg)](https://github.com/EtixLabs/CES/releases/latest)

### RTSP server with authentication for testing purposes

## Dependencies

* `docker`

## Examples of use

To create a simple test stream, just launch `docker run -p 8554:8554 ullaakut/CES` and access it on the URL `rtsp://0.0.0.0:8554/live.sdp`

To create a test stream with credentials, just add the RTSP_PASSWORD and RTSP_USERNAME arguments like so : `docker run -p 8554:8554 -e RTSP_PASSWORD=mypass -e RTSP_USERNAME=myusername` and then access it on the URL `rtsp://myusername:mypass@0.0.0.0:8554/live.sdp`.

### Usage

`docker run [-e RTSP_LISTEN_ADDRESS=your_listen_address] [-e RTSP_PORT=your_port -p your_port:your_port] [-e RTSP_PATH=your_path] [-e RTSP_INPUT_STREAM=your_input_stream] [-e RTSP_USERNAME=your_username] [-e RTSP_PASSWORD=your_password] [-e RTSP_RESOLUTION='your_width'x'your_height'] [-e RTSP_FRAMERATE=your_framerate] [-e GST_DEBUG=your_debug_level] ullaakut/CES`

### Parameters

All of these options override the default parameters for CES
* `your_listen_address`: The address you want your server to listen on [default: `0.0.0.0`]
* `your_port`: The port that you want your server to listen on [default: `8554`] _Don't forget to also expose the port in your container with the -p option like in the example above_
* `your_path`: The rtsp path at which you want your stream to be served [default: `/live.sdp`]
* `your_input_stream`: The stream you want to broadcast using CES [default: none]
* `your_username`: If you want to enable security on your stream, using this option will allow you to specify the username required to access your stream [default: none]
* `your_password`: If you want to enable security on your stream, using this option will allow you to specify the password required to access your stream [default: none]
* `'your_width'x'your_height'`: The resolution at which you want to stream [default: `352x288`]
* `your_framerate`: The desired output framerate for your stream [default: `25`]
* `your_debug_level`: The desired debug level for GStreamer [default: none] _See [this link](https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/gst-running.html) for more information on this variable_

## License

Copyright 2017 Etix Labs

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.
