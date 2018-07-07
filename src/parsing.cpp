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
#include <iostream>
#include <string.h>

// Set config defaults or from env
void parse_env(std::shared_ptr<t_config> &config) {
  // Address
  config->address = DEFAULT_ADDRESS;
  if (const char *address = std::getenv("RTSP_ADDRESS")) {
    config->address = address;
  }

  // Port
  config->port = DEFAULT_PORT;
  if (const char *port = std::getenv("RTSP_PORT")) {
    config->port = port;
  }

  // Route
  config->route = DEFAULT_ROUTE;
  if (const char *route = std::getenv("RTSP_ROUTE")) {
    config->route = route;
  }

  // Username
  config->username = DEFAULT_USERNAME;
  if (const char *username = std::getenv("RTSP_USERNAME")) {
    config->username = username;
  }

  // Password
  config->password = DEFAULT_PASSWORD;
  if (const char *password = std::getenv("RTSP_PASSWORD")) {
    config->password = password;
  }

  // Framerate
  config->framerate = "";
  if (const char *framerate = std::getenv("RTSP_FRAMERATE")) {
    config->framerate = framerate;
  }

  // Scale
  config->scale =
      std::make_pair<std::string, std::string>("", "");
  if (const char *scale = std::getenv("RTSP_RESOLUTION")) {
    size_t pos = 0;
    std::string scale_str(scale);

    if ((pos = scale_str.find("x")) == std::string::npos) {
      std::cerr
          << "No x token found between width and height in the scale argument: "
          << scale_str << std::endl
          << "Using default values";
    } else {
      config->scale = std::make_pair<std::string, std::string>(
          scale_str.substr(0, pos), scale_str.substr(pos + 1));
    }
  }

  // Input
  config->input = DEFAULT_INPUT;
  if (const char *input = std::getenv("INPUT")) {
    config->input = input;
  }

  // Time
  config->time = DEFAULT_TIME_ENABLED;
  if (const char *time = std::getenv("ENABLE_TIME_OVERLAY")) {
    if (strcmp(time, "false") == 0) {
      config->time = false;
    } else {
      config->time = true;
    }
  }

  // Digest auth (if anything other than digest, basic will be used)
  config->digest = DEFAULT_DIGEST_ENABLED;
  if (const char *auth = std::getenv("RTSP_AUTHENTICATION_METHOD")) {
    if (strcmp(auth, "digest") == 0) {
      config->digest = true;
    } else {
      config->digest = false;
    }
  }

  // Pipeline
  config->pipeline = DEFAULT_GST_PIPELINE;
  if (const char *pipeline = std::getenv("GST_PIPELINE")) {
    config->pipeline = pipeline;
  }
}

// Overwrite default parameters via cmd line
bool parse_args(std::shared_ptr<t_config> &config, int argc, char **argv) {
  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "r:u:l:p:b:f:s:i:g:ht")) != -1) {
    switch (c) {
    case 'r': // Route
      if (optarg && optarg[0] == '-') {
        break;
      }
      if (not optarg[0] == '/') {
        config->route = "/";
      }
      config->route += optarg;
      break;
    case 'u': // Username
      if (optarg && optarg[0] == '-') {
        break;
      }
      config->username = optarg;
      break;
    case 'p': // Password
      if (optarg && optarg[0] == '-') {
        break;
      }
      config->password = optarg;
      break;
    case 'i': // Input
      if (optarg && optarg[0] == '-') {
        break;
      }
      config->input = optarg;
      break;
    case 'l': // Listen address
      if (optarg && optarg[0] == '-') {
        break;
      }
      config->address = optarg;
      break;
    case 'b': // Port
      if (optarg && optarg[0] == '-') {
        break;
      }
      config->port = optarg;
      break;
    case 'f': // Framerate
      if (optarg && optarg[0] == '-') {
        break;
      }
      config->framerate = optarg;
      break;
    case 's': { // Scale
      if (optarg && optarg[0] == '-') {
        break;
      }
      size_t pos = 0;
      std::string scale = optarg;
      if ((pos = scale.find("x")) == std::string::npos) {
        std::cerr << "No x token found between width and height in the scale "
                     "argument: "
                  << scale << std::endl
                  << "Using default values instead";
        return false;
      }
      config->scale.first = scale.substr(0, pos);
      config->scale.second = scale.substr(pos + 1);
      break;
    }
    case 't': // Time Overlay
      config->time = true;
      break;
    case 'd': // Use digest instead of basic auth
      config->digest = true;
      break;
    case 'g': // Force custom gstreamer pipeline
      if (optarg && optarg[0] == '-') {
        break;
      }
      config->pipeline = optarg;
      break;
    case 'h': // help
      fprintf(stdout,
              "Usage: %s [-l address] [-b port] [-r route] [-i "
              "input] [-u username] [-p password] [-f framerate] [-s "
              "'width'x'height'] [-d] [-t] [-h] [-g gst-pipeline]\n",
              argv[0]);
      return true;
    case '?':
      if (optopt == 'r' || optopt == 'l' || optopt == 'p' || optopt == 'u' ||
          optopt == 'i' || optopt == 'a' || optopt == 'b' || optopt == 'f' ||
          optopt == 's' || optopt == 'g') {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      } else if (isprint(optopt)) {
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      } else {
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      }
      return false;
    default:
      return false;
      ;
    }
  }
  return true;
}

void parse_input_type(std::shared_ptr<t_config> &config) {
  if (config->input.compare(0, 7, "rtsp://") == 0) { // RTSP stream input
    config->input_type = RTSP_INPUT;
  } else if (config->input.empty() || config->input.compare(0, 8,
                                                            "pattern:") ==
                                          0) { // Videotestsrc pattern input
    config->input_type = VIDEOTESTSRC_INPUT;
  } else if (config->input.compare(0, 10, "/dev/video") == 0) { // v4l2src input
    config->input_type = DEVICE_INPUT;
  } else { // File
    config->input_type = FILE_INPUT;
  }
}

std::string input_type_to_string(InputType type) {
  switch (type) {
  case UNDEFINED_INPUT:
    return "undefined";
  case FILE_INPUT:
    return "file";
  case RTSP_INPUT:
    return "rtsp";
  case VIDEOTESTSRC_INPUT:
    return "videotestsrc";
  case DEVICE_INPUT:
    return "v4l2src";
  default:
    break;
  }
}

void dump_config(std::shared_ptr<t_config> &config) {
  // Server config
  std::cout << "Server configuration:" << std::endl
            << "Address:\t" << config->address << std::endl
            << "Port:\t\t" << config->port << std::endl
            << "Route:\t\t" << config->route << std::endl
            << "Username:\t" << config->username << std::endl
            << "Password:\t" << config->password << std::endl
            << "Auth method:\t";

            if (config->digest) {
              std::cout << "digest";
            } else {
              std::cout << "basic";
            }

           std::cout << std::endl;

  // Input
  std::cout << "Input:\t\t";
  config->input.empty() ? std::cout << "pattern:smpte" << std::endl
                        : std::cout << config->input << std::endl;
  std::cout << "Input type:\t" << input_type_to_string(config->input_type)
            << std::endl
            << std::endl;
}
