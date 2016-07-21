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

#include <server.h>

static gboolean remove_func(GstRTSPSessionPool *pool, GstRTSPSession *session,
                            GstRTSPServer *server) {
  return GST_RTSP_FILTER_REMOVE;
}

static gboolean remove_sessions(GstRTSPServer *server) {
  GstRTSPSessionPool *pool;

  g_print("removing all sessions\n");
  pool = gst_rtsp_server_get_session_pool(server);
  gst_rtsp_session_pool_filter(pool, (GstRTSPSessionPoolFilterFunc)remove_func,
                               server);
  g_object_unref(pool);

  return FALSE;
}

static gboolean timeout(GstRTSPServer *server) {
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool(server);
  gst_rtsp_session_pool_cleanup(pool);
  g_object_unref(pool);

  return true;
}

void init(t_server *serv) {
  serv->path = strdup("");
  serv->username = strdup("");
  serv->password = strdup("");
}

void init_server_auth(t_server *serv) {
  serv->loop = g_main_loop_new(NULL, FALSE);

  /* create a server instance */
  serv->server = gst_rtsp_server_new();

  /* get the mounts for this server, every server has a default mapper object
   * that be used to map uri mount points to media factories */
  serv->mounts = gst_rtsp_server_get_mount_points(serv->server);

  /* make a media factory for a test stream. The default media factory can use
   * gst-launch syntax to create pipelines.
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  serv->factory = gst_rtsp_media_factory_new();
  gst_rtsp_media_factory_set_launch(
      serv->factory,
      "( "
      "videotestsrc ! video/x-raw,width=352,height=288,framerate=15/1 ! "
      "x264enc ! rtph264pay name=pay0 pt=96 "
      "audiotestsrc ! audio/x-raw,rate=8000 ! "
      "alawenc ! rtppcmapay name=pay1 pt=97 "
      ")");
  /* attach the test factory to the /test url */
  gst_rtsp_mount_points_add_factory(serv->mounts, serv->path, serv->factory);

  if (strlen(serv->username)) {
    /* the user can look at the media but not construct so he gets a
    * 401 Unauthorized */
    gst_rtsp_media_factory_add_role(
        serv->factory, serv->username, GST_RTSP_PERM_MEDIA_FACTORY_ACCESS,
        G_TYPE_BOOLEAN, true, GST_RTSP_PERM_MEDIA_FACTORY_CONSTRUCT,
        G_TYPE_BOOLEAN, true, NULL);
  }
  /* don't need the ref to the mapper anymore */
  g_object_unref(serv->mounts);

  if (strlen(serv->username)) {
    /* make a new authentication manager */
    serv->auth = gst_rtsp_auth_new();

    /* make admin token */
    serv->token = gst_rtsp_token_new(GST_RTSP_TOKEN_MEDIA_FACTORY_ROLE,
                                     G_TYPE_STRING, serv->username, NULL);
    serv->basic = gst_rtsp_auth_make_basic(serv->username, serv->password);
    gst_rtsp_auth_add_basic(serv->auth, serv->basic, serv->token);
    g_free(serv->basic);
    gst_rtsp_token_unref(serv->token);

    /* set as the server authentication manager */
    gst_rtsp_server_set_auth(serv->server, serv->auth);
    g_object_unref(serv->auth);
  }
}

int server_launch(t_server *serv) {
  /* attach the server to the default maincontext */
  if (gst_rtsp_server_attach(serv->server, NULL) == 0)
    goto failed;

  g_timeout_add_seconds(2, (GSourceFunc)timeout, serv->server);
  g_timeout_add_seconds(10, (GSourceFunc)remove_sessions, serv->server);

  g_print("stream ready at rtsp://");
  /* start serving */
  if (strlen(serv->username) > 0)
    g_print("%s:%s@", serv->username, serv->password);

  g_print("127.0.0.1:8554%s\n", serv->path);

  g_main_loop_run(serv->loop);

/* ERRORS */
failed : {
  g_print("failed to attach the server\n");
  return -1;
}
  return 0;
}

int main(int argc, char *argv[]) {
  t_server serv;
  int c;

  gst_init(NULL, NULL);
  init(&serv);

  opterr = 0;
  while ((c = getopt(argc, argv, "r:u:p:h")) != -1)
    switch (c) {
    case 'r':
      if (optarg[0] == '/')
        serv.path = strdup(optarg);
      else
        serv.path = strcat(strdup("/"), strdup(optarg));
      break;
    case 'u':
      serv.username = strdup(optarg);
      break;
    case 'p':
      serv.password = strdup(optarg);
      break;
    case 'h':
      fprintf(stdout, "Usage: ./etix_rtsp_server [-r route] [-u username] [-p password]\n");
      break;
    case '?':
      if (optopt == 'r' || optopt == 'p' || optopt == 'u')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return 1;
    default:
      abort();
    }
  init_server_auth(&serv);

  return server_launch(&serv);
}
