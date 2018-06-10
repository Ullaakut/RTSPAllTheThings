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

/* Init gst rtsp server */
bool server_init(t_server *serv) {
  serv->loop = g_main_loop_new(NULL, FALSE);

  /* create a server instance */
  serv->server = gst_rtsp_server_new();

  /* get the mounts for this server, every server has a default mapper object
   * that be used to map uri mount points to media factories */
  serv->mounts = gst_rtsp_server_get_mount_points(serv->server);

  /* Set the address and port to bind */
  gst_rtsp_server_set_address(serv->server, serv->config->address.c_str());
  gst_rtsp_server_set_service(serv->server, serv->config->port.c_str());

  auto &&session = gst_rtsp_session_new("New session");
  gst_rtsp_session_prevent_expire(session);

  serv->factory = gst_rtsp_media_factory_new();

  gst_rtsp_media_factory_set_shared(serv->factory, TRUE);
  gst_rtsp_media_factory_set_latency(serv->factory, 0);

  /* attach the test factory to the given path */
  gst_rtsp_mount_points_add_factory(serv->mounts, serv->config->route.c_str(),
                                    serv->factory);

  /* don't need the ref to the mapper anymore */
  g_object_unref(serv->mounts);

  if (not serv->config->username.empty()) {
    /* the user can look at the media but not construct so he gets a
     * 401 Unauthorized */
    gst_rtsp_media_factory_add_role(
        serv->factory, serv->config->username.c_str(),
        GST_RTSP_PERM_MEDIA_FACTORY_ACCESS, G_TYPE_BOOLEAN, true,
        GST_RTSP_PERM_MEDIA_FACTORY_CONSTRUCT, G_TYPE_BOOLEAN, true, NULL);

    /* make a new authentication manager */
    serv->auth = gst_rtsp_auth_new();

    /* make admin token */
    serv->token =
        gst_rtsp_token_new(GST_RTSP_TOKEN_MEDIA_FACTORY_ROLE, G_TYPE_STRING,
                           serv->config->username.c_str(), NULL);

  if (serv->config->digest) {
    gst_rtsp_auth_set_supported_methods(serv->auth, GST_RTSP_AUTH_DIGEST);
    gst_rtsp_auth_add_digest(serv->auth, serv->config->username.c_str(), serv->config->password.c_str(), serv->token);
  } else {
    serv->basic = gst_rtsp_auth_make_basic(serv->config->username.c_str(),
                                           serv->config->password.c_str());
    gst_rtsp_auth_add_basic(serv->auth, serv->basic, serv->token);
    g_free(serv->basic);
  }
    gst_rtsp_token_unref(serv->token);

    /* set as the server authentication manager */
    gst_rtsp_server_set_auth(serv->server, serv->auth);
    g_object_unref(serv->auth);
  }

  /* Pipeline launch */
  std::string launchCmd = create_pipeline(serv->config);
  if (serv->config->input_type == FILE_INPUT) {
    if (!configure_file_input(serv)) {
      return false;
    }
  }
  g_print("Launching stream with the following pipeline: %s\n",
          launchCmd.c_str());
  gst_rtsp_media_factory_set_launch(serv->factory, launchCmd.c_str());

  return true;
}

/* Launch gst rtsp server */
int server_launch(t_server *serv) {
  /* attach the server to the default maincontext */
  if (gst_rtsp_server_attach(serv->server, NULL) == 0) {
    goto failed;
  }

  g_print("Stream ready at rtsp://");
  /* start serving */
  if (not serv->config->username.empty()) {
    g_print("%s:%s@", serv->config->username.c_str(),
            serv->config->password.c_str());
  }

  g_print("%s:%s%s\n", serv->config->address.c_str(),
          serv->config->port.c_str(), serv->config->route.c_str());

  g_main_loop_run(serv->loop);

/* ERRORS */
failed : {
  g_print("failed to attach the server\n");
  return -1;
}
  return 0;
}
