/* profzen-server.h */

#include "../../libwebsockets.h/build/lws_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fnctl.h>
#include <assert.h>

#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>

#include "../../lib/libwebsockets.h"

struct per_session_data_profzen_writer {
};

struct per_session_data_profzen_annotator {
};


extern int
callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);


extern int
callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

extern int
callback_profzen_writer(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

extern int
callback_profzen_annotator(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
