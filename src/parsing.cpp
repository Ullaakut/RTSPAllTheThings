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
#include <string.h>

void parse_env(std::shared_ptr<t_config> config) {
  if (const char *route = std::getenv("RTSP_PATH"))
    config->route = strdup(route);
  else
    config->route = strdup("/live.sdp");

  if (const char *username = std::getenv("RTSP_USERNAME"))
    config->username = strdup(username);
  else
    config->username = strdup("");

  if (const char *password = std::getenv("RTSP_PASSWORD"))
    config->password = strdup(password);
  else
    config->password = strdup("");

  if (const char *input = std::getenv("RTSP_INPUT_FILE"))
    config->input = strdup(input);
  else
    config->input = strdup("");

  if (const char *scale = std::getenv("RTSP_RESOLUTION")) {
    size_t pos = 0;
    std::string scale_str(scale);

    if ((pos = scale_str.find("x")) == std::string::npos) {
      fprintf(stderr, "No x token found between width and height in the scale "
                      "argument: %s\nUsing default values instead",
              scale);
      config->scale =
          std::make_pair<gchar *, gchar *>(strdup("352"), strdup("288"));
    } else {
      config->scale = std::make_pair<gchar *, gchar *>(
          strdup(scale_str.substr(0, pos).c_str()),
          strdup(scale_str.substr(pos + 1).c_str()));
    }
  } else {
    config->scale =
        std::make_pair<gchar *, gchar *>(strdup("352"), strdup("288"));
  }

  if (const char *framerate = std::getenv("RTSP_FRAMERATE"))
    config->framerate = strdup(framerate);
  else
    config->framerate = strdup("25");

  if (const char *address = std::getenv("RTSP_ADDRESS"))
    config->address = strdup(address);
  else
    config->address = strdup("0.0.0.0");

  if (const char *port = std::getenv("RTSP_PORT"))
    config->port = strdup(port);
  else
    config->port = strdup("8554");
}
