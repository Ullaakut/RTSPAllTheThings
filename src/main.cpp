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
#include <cstring>
#include <iostream>

void signal_handler(int signal) {
  std::cout << "Signal " << std::to_string(signal) << " catched" << std::endl;
  exit(1);
}

int main(int argc, char **argv) {
  std::signal(SIGINT, signal_handler);

  t_server serv;
  std::shared_ptr<t_config> config = std::make_shared<t_config>();
  serv.config = config;

  /* Config parsing from env */
  parse_env(config);

  /* Config parsing from args (overwrite) */
  if (not parse_args(config, argc, argv)) {
    std::cerr << "Unable to parse arguments" << std::endl;
    return -1;
  }
  parse_input_type(config);

  print_logo();
  print_name();
  dump_config(config);

  /* Init and launch server */
  gst_init(NULL, NULL);
  if (!server_init(&serv)) {
    std::cerr << "Input file not found at " << config->input << std::endl;
    return -1;
  }
  return server_launch(&serv);
}
