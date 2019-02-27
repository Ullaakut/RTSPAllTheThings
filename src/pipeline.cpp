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

// If time overlay is enabled, add it to the pipeline
std::string time_overlay(std::shared_ptr<t_config> &config) {
  if (config->time) {
    return " ! timeoverlay halignment=left valignment=top "
           "shaded-background=true "
           "font-desc=\"Sans 10\" ! "
           "clockoverlay halignment=right valignment=top "
           "shaded-background=true "
           "font-desc=\"Sans 10\"";
  }
  return "";
}

bool needs_encoding(std::shared_ptr<t_config> &config) {
  return (config->framerate != "" ||
      config->scale.first != "" ||
      config->scale.second != "" ||
      config->input_type == VIDEOTESTSRC_INPUT ||
      config->input_type == FILE_INPUT||
      config->input_type == DEVICE_INPUT);
}

bool needs_resize_or_rerate(std::shared_ptr<t_config> &config) {
  return (config->framerate != "" ||
      config->scale.first != "" ||
      config->scale.second != "");
}

// Take raw, change caps according to conf and transcode in h264
std::string encode(std::shared_ptr<t_config> &config) {
  std::string launchCmd = "";

    if (needs_resize_or_rerate(config)) {
      std::cout << "H264 encoding with:" << std::endl;
    }

  // Change caps & encode
  if (config->scale.first != "" &&
      config->scale.second != "") {
    std::cout << "Resolution:\t" << config->scale.first << "x"
              << config->scale.second << std::endl
              << std::endl;
    launchCmd += " ! videoscale ! video/x-raw,width=";
    launchCmd += config->scale.first;
    launchCmd += ",height=";
    launchCmd += config->scale.second;
  } else if (config->input_type == VIDEOTESTSRC_INPUT) {
    std::cout << "Resolution:\t" << DEFAULT_WIDTH << "x"
              << DEFAULT_HEIGHT << std::endl
              << std::endl;
    launchCmd += " ! videoscale ! video/x-raw,width=";
    launchCmd += DEFAULT_WIDTH;
    launchCmd += ",height=";
    launchCmd += DEFAULT_HEIGHT;
  }

  if (config->framerate != "") {
    std::cout << "Framerate:\t" << config->framerate << std::endl;

    launchCmd += " ! videorate ! video/x-raw,framerate=";
    launchCmd += config->framerate;
    launchCmd += "/1";
  }

  if (needs_encoding(config)) {
    launchCmd += " ! capsfilter ! queue ! x264enc speed-preset=superfast";
  }

  return launchCmd;
}

// Rtsp input pipeline
std::string create_rtsp_input(std::shared_ptr<t_config> &config) {
  std::string launchCmd = "";

  // Receive & depay
  launchCmd += "rtspsrc latency=0 auto-start=true location=";
  launchCmd += config->input;
  launchCmd += " ! rtph264depay";

  if (needs_resize_or_rerate(config)) {
    // If user request modification in framerate or scale -> encode
    launchCmd += " ! h264parse ! avdec_h264"; // Decode
  }

  launchCmd += time_overlay(config);
  launchCmd += encode(config);
  return launchCmd;
}

// Videosrc input pipeline
std::string create_videotestsrc_input(std::shared_ptr<t_config> &config) {
  std::string launchCmd = "";

  launchCmd += "videotestsrc ";
  if (config->input.compare(0, 8, "pattern:") == 0) {
    launchCmd += "pattern=";
    launchCmd += config->input.substr(8);
  }

  launchCmd += time_overlay(config);
  launchCmd += encode(config);
  return launchCmd;
}

// File input pipeline
std::string create_file_input(std::shared_ptr<t_config> &config) {
  std::string launchCmd = "";

  launchCmd += "appsrc name=mysrc";
  launchCmd += " ! decodebin";

  launchCmd += time_overlay(config);
  launchCmd += encode(config);
  return launchCmd;
}

// Device input pipeline
std::string create_device_input(std::shared_ptr<t_config> &config) {
  std::string launchCmd = "";

  launchCmd += " v4l2src device=";
  launchCmd += config->input;

  launchCmd += time_overlay(config);
  launchCmd += encode(config);
  return launchCmd;
}

/* Create pipeline according to config */
std::string create_pipeline(std::shared_ptr<t_config> &config) {
  std::string launchCmd = "( ";

  if (config->pipeline != "") {
    launchCmd += config->pipeline;
  } else {
    if (config->input_type == RTSP_INPUT) {
      launchCmd += create_rtsp_input(config);
    } else if (config->input_type == VIDEOTESTSRC_INPUT) {
      launchCmd += create_videotestsrc_input(config);
    } else if (config->input_type == FILE_INPUT) {
      launchCmd += create_file_input(config);
    } else if (config->input_type == DEVICE_INPUT) {
      launchCmd += create_device_input(config);
    }

    launchCmd += " ! rtph264pay name=pay0 pt=96 ";
  }

  launchCmd += " )";
  return launchCmd;
}
