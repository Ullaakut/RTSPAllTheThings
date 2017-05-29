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
#include <csignal>
#include <iostream>

void signal_handler(int signal) {
  std::cout << "Signal " << std::to_string(signal) << " catched" << std::endl;
  exit(1);
}

int main(int argc, char **argv) {
  std::signal(SIGINT, signal_handler);

  /* Config parsing from env */
  t_server serv;
  std::shared_ptr<t_config> config = std::make_shared<t_config>();
  parse_env(config);
  serv.config = config;

  gst_init(NULL, NULL);
  init_server_auth(&serv);
  return server_launch(&serv);
}
