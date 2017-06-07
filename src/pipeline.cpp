// Copyright 2016 Etix Labs
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "server.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string.h>

// Take raw, change caps according to conf and transcode in h264
std::string encode(std::shared_ptr<t_config> config) {
  std::cout << "H264 encoding with: " << std::endl
            << "Framerate : " << config->framerate << std::endl
            << "Resolution: " << config->scale.first << "x"
            << config->scale.second << std::endl
            << std::endl;

  std::string launchCmd = "";

  // Change caps & encode
  launchCmd += " ! videoscale ! video/x-raw,width=";
  launchCmd += config->scale.first;
  launchCmd += ",height=";
  launchCmd += config->scale.second;
  launchCmd += " ! videorate ! video/x-raw,framerate=";
  launchCmd += config->framerate;
  launchCmd += "/1";
  launchCmd += " ! capsfilter ! queue"
               " ! x264enc speed-preset=superfast "
               "tune=zerolatency";

  return launchCmd;
}

// Rtsp input pipeline
std::string create_rtsp_input(std::shared_ptr<t_config> config) {
  std::string launchCmd = "";

  // Receive & depay
  launchCmd += "rtspsrc latency=0 auto-start=true location=";
  launchCmd += config->input;
  launchCmd += " ! rtph264depay";

  // If user request modification in framerate or scale -> encode
  if (config->framerate != DEFAULT_FRAMERATE ||
      config->scale.first != DEFAULT_WIDTH ||
      config->scale.second != DEFAULT_HEIGHT) {
    launchCmd += " ! h264parse ! avdec_h264"; // Decode
    launchCmd += encode(config);
  }
  return launchCmd;
}

// Videosrc input pipeline
std::string create_videotestsrc_input(std::shared_ptr<t_config> config) {
  std::string launchCmd = "";

  launchCmd += "videotestsrc ";
  if (config->input.compare(0, 8, "pattern:") == 0) {
    launchCmd += "pattern=";
    launchCmd += config->input.substr(8);
  }
  launchCmd += encode(config);
  return launchCmd;
}

// File input pipeline
std::string create_file_input(std::shared_ptr<t_config> config) {
  std::string launchCmd = "";

  launchCmd += "multifilesrc loop=true location=";
  launchCmd += config->input;
  launchCmd += " ! decodebin";
  launchCmd += encode(config);
  return launchCmd;
}

/* Create pipeline according to config */
std::string create_pipeline(std::shared_ptr<t_config> config) {
  std::string launchCmd = "( ";

  if (config->input.compare(0, 5, "rtsp:") == 0) { // RTSP stream input
    launchCmd += create_rtsp_input(config);
  } else if (config->input.empty() ||
             config->input.compare(0, 8,
                                   "pattern:") ==
                 0) { // Videotestsrc pattern input
    launchCmd += create_videotestsrc_input(config);
  } else { // File
    launchCmd += create_file_input(config);
  }

  launchCmd += " ! rtph264pay name=pay0 pt=96 ";
  launchCmd += " )";
  return launchCmd;
}
