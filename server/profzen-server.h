/* profzen-server.h */

#include "lws_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>

#include "libwebsockets.h"

extern volatile int force_exit;
extern struct lws_context *context;
extern char *resource_path;

struct per_session_data__http {
	lws_filefd_type	fd;
	int reason_bf;
	unsigned int client_finished:1;
};


struct per_session_data__profzen_writer {
};

struct per_session_data__profzen_annotator {
};


extern int
callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);


extern int
callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

extern int
callback_profzen_writer(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

extern int
callback_profzen_annotator(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
