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

#pragma once

#include <gst/rtsp-server/rtsp-server.h>
#include <memory>

typedef struct s_config {
  gchar *username;
  gchar *password;
  gchar *route;
  gchar *address;
  gchar *port;
  gchar *input;
  gchar *framerate;
  std::pair<gchar *, gchar *> scale;
} t_config;

typedef struct s_server {
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  GstRTSPAuth *auth;
  GstRTSPToken *token;
  gchar *basic;
  std::shared_ptr<t_config> config;
} t_server;

void init(t_server *serv);
int parse_args(std::shared_ptr<t_config> config, int argc, char **argv);
void parse_env(std::shared_ptr<t_config> config);
void init_server_auth(t_server *serv);
int server_launch(t_server *serv);
