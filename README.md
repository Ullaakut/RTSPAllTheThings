# RTSPATT : RTSP All The Things 1.3.1

[![License](https://img.shields.io/badge/license-Apache-blue.svg)](#license)
[![Build](https://img.shields.io/travis/EtixLabs/RTSPAllTheThings/master.svg?style=flat)](https://travis-ci.org/Ullaakut/RTSPAllTheThings)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/86681212bbc04cf797640dbe700e7057)](https://www.codacy.com/app/brendan-le-glaunec/RTSPAllTheThings?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=EtixLabs/RTSPAllTheThings&amp;utm_campaign=Badge_Grade)
[![Docker Pulls](https://img.shields.io/docker/pulls/ullaakut/rtspatt.svg?style=flat)](https://hub.docker.com/r/ullaakut/rtspatt/)
[![Latest release](https://img.shields.io/github/release/EtixLabs/RTSPAllTheThings.svg?style=flat)](https://github.com/EtixLabs/RTSPAllTheThings/releases/latest)

**A multipurpose RTSP media server that can simulate RTSP cameras, broadcast RTSP streams, webcams and even create test videos or serve video files.**

## Dependencies

* `docker`

## Usage from the official docker repository

You can create a stream by launching the official docker image:
`docker run --rm -p 8554:8554 ullaakut/rtspatt`

With default options, stream will be available at `rtsp://0.0.0.0:8554/live.sdp`.
You can use [override options](#override-options).

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
       [-e ENABLE_TIME_OVERLAY=true|false] \
       [-e RTSP_AUTHENTICATION_METHOD=digest|basic] \
       [-e GST_DEBUG=your_debug_level] \
       ullaakut/rtspatt
```

```
./rtspatt \
      [-l rtsp_address] \
      [-b rtsp_port] \
      [-r rtsp_route] \
      [-u rtsp_username] \
      [-p rtsp_password] \
      [-s rtsp_resolution] \
      [-f rtsp_framerate] \
      [-t] \
      [-d] \
      [-i input]
```

To have GStreamer debug while using the binary, simply run `export GST_DEBUG=your_debug_level` before using RTSP All The Things.

All of these environment variables and command line arguments override the default parameters:
* `RTSP_ADDRESS` | `-l`:
  The address you want your server to listen on [default: `0.0.0.0`]
* `RTSP_PORT` | `-b`:
  The port that you want your server to listen on [default: `8554`] (_don't forget to also expose the port in your docker container with the `-p` docker option like in the example above_)
* `RTSP_ROUTE` | `-r`:
  The rtsp route at which you want your stream to be served [default: `/live.sdp`]
* `RTSP_USERNAME` | `-u`:
  If you want to enable security on your stream, using this option will allow you to specify the username required to access your stream [default: none]
* `RTSP_PASSWORD` | `-p`:
  If you want to enable security on your stream, using this option will allow you to specify the password required to access your stream [default: none]
* `RTSP_RESOLUTION` | `-s`:
  The resolution at which you want to stream [default: `1280x720`] - RTSPATT will have to do encoding to resize the stream (Increased CPU usage)
* `RTSP_FRAMERATE` | `-f`:
  The desired output framerate for your stream [default: `25`] - RTSPATT will have to do encoding to change the framerate (Increased CPU usage)
* `INPUT` | `-i`:
  Input used as video source. [default: `pattern:smtpe`]
  - If the argument starts with `rtsp://` it will try to open it as an **RTSP stream**
  - If it starts with `pattern:` if will create a **test video with the given pattern** (_see [this link](https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-base-plugins/html/gst-plugins-base-plugins-videotestsrc.html#GstVideoTestSrcPattern) for more information on this argument_)
  - If the argument starts with `/dev/video` it will create a stream from the /dev/video device passed into docker at the launch of the instance. You will need to pass in the video device as follows: `--device=/dev/video0:/dev/video0` where `video0` is the video device.
  - Otherwise it will use the argument as a absolute link to **file input**. Remember to use the `-v` option to insert a video into your container if you want to read it. Example: `docker run -e INPUT=/tmp/video.mp4 -v /home/test/documents/myVideo.mp4:/tmp/video -p 8554:8554 ullaakut/rtspatt`. The repository contains a 30 second sample video in the `samples` folder.
* `ENABLE_TIME_OVERLAY` | `-t`:
  If the environment variable is set to true or the command line flag is used, a time overlay will be added to the stream. This can be useful to debug latency. [default: disabled] - RTSPATT will have to do encoding to add the time (Increased CPU usage)
* `RTSP_AUTHENTICATION_METHOD` | `-d`:
  If RTSP_AUTHENTICATION_METHOD is set to `digest` or the command line flag is used, the authentication will be done using digest instead of basic auth. [default: basic]
* `GST_DEBUG`:
  The desired debug level for GStreamer [default: none] (_see [this link](https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/gst-running.html) for more information on this variable_)

## Examples

<p align="center"><img src="https://preview.ibb.co/emxVna/Example_Screen_Shot.png" alt="Ball"></p>

> Serve video using basic auth on `rtsp:admin:test@//0.0.0.0:8554/live.sdp`

`docker run --rm -e RTSP_USERNAME=admin  -e RTSP_PASSWORD=test -p 8554:8554 rtspatt`

> Serve video using digest auth on `rtsp:admin:test@//0.0.0.0:8554/live.sdp`

`docker run --rm -e RTSP_AUTHENTICATION_METHOD=digest -e RTSP_USERNAME=admin  -e RTSP_PASSWORD=test -p 8554:8554 rtspatt`

> Launch an RTSP stream on `rtsp://0.0.0.0:8554/live.sdp` with a snow pattern and a resolution of 960x600 pixels:

`docker run --rm -e INPUT="pattern:snow" -e RTSP_RESOLUTION=960x600 -p 8554:8554 ullaakut/rtspatt` or `./rtspatt -i pattern:snow -s 960x600`

> Broadcast a camera's stream and change its framerate to 12 frames per second:

`docker run --rm -e INPUT="rtsp://root:root@camera_ip:554/live.sdp" -e RTSP_FRAMERATE=12 -p 8554:8554 ullaakut/rtspatt` or `./rtspatt -i rtsp://root:root@camera_ip:554/live.sdp -f 12`

> Broadcast a video stream from a connected device:

`docker run --rm -e INPUT="/dev/video0" --device=/dev/video0:/dev/video0 -p 8554:8554 rtspatt`

> Serve a video file on a specific address and route:

`docker run --rm -e INPUT="/tmp/video.avi" -e RTSP_ADDRESS=172.100.100.12 -e RTSP_PORT=18554 -v "/path/to/your/video:/tmp/video.avi" -p 18554:18554 ullaakut/rtspatt` or `./rtspatt -i /tmp/video.avi -l 172.100.100.12 -b 18554`

## Build

You can modify RTSPATT and create your own docker image. For this simply run:
`./build.sh`

This script will use a docker image that has all the necessary dependencies to build the solution, and will output the binary once it's ready. It will also use this to build the RTSPATT docker image itself.

Once it's done, you can launch RTSPATT with:
`docker run --rm -p 8554:8554 rtspatt` or `./rtspatt`.

With default options, stream will be available at `rtsp://0.0.0.0:8554/live.sdp`.
You can use [override options](#override-options).

## Patterns

Here is the list of patterns you can use

* `pattern:smpte`               - The standard SMTPE test pattern, it's the default value if you don't specify any input
* `pattern:smpte-75`            - 75% color bars
* `pattern:smpte-100`           - 100% color bars (no noise)
* `pattern:snow`                - Random noise
* `pattern:black`               - Black image
* `pattern:white`               - White image
* `pattern:red`                 - Red image
* `pattern:green`               - Green image
* `pattern:blue`                - Blue image
* `pattern:checkers-1`          - 1 pixel checkers pattern (low stream scale recommended)
* `pattern:checkers-2`          - 2 pixel checkers pattern (low stream scale recommended)
* `pattern:checkers-4`          - 4 pixel checkers pattern (low stream scale recommended)
* `pattern:checkers-8`          - 8 pixel checkers pattern (low stream scale recommended)
* `pattern:circular`            - circular pattern
* `pattern:blink`               - Alternate between black and while image
* `pattern:zone-plate`          - Zone plate
* `pattern:gamut`               - Gamut checking pattern
* `pattern:chroma-zone-plate`   - Chroma zone plate
* `pattern:ball`                - A moving ball
* `pattern:bar`                 - Bar with foreground color
* `pattern:pinwheel`            - Pinwheel
* `pattern:spokes`              - Spokes
* `pattern:gradient`            - Gradient image
* `pattern:colors`              - All colors

<p align="center"><img width="200" height="200" src="https://image.ibb.co/nAjMtF/Ball.png" alt="Ball"><img width="200" height="200" src="https://image.ibb.co/dGZbSa/Bar.png" alt="Bar"><img width="200" height="200" src="https://image.ibb.co/bFenYF/Checkers.png" alt="Checkers"></br><img width="200" height="200" src="https://image.ibb.co/ekBC0v/Circular.png" alt="Circular"><img width="200" height="200" src="https://image.ibb.co/j8UMtF/Colors.png" alt="Colors"><img width="200" height="200" src="https://image.ibb.co/bCPX0v/Gamut.png" alt="Gamut"></br><img width="200" height="200" src="https://image.ibb.co/dd3QLv/Gradient.png" alt="Gradient"><img width="200" height="200" src="https://image.ibb.co/g7Ai7a/Pinwheel.png" alt="Pinwheel"><img width="200" height="200" src="https://image.ibb.co/gu7Kfv/SMPTE.png" alt="SMPTE"></br><img width="200" height="200" src="https://image.ibb.co/gnBefv/Snow.png" alt="Snow"><img width="200" height="200" src="https://image.ibb.co/mLjMtF/Spokes.png" alt="Spokes"></p>

## License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.
