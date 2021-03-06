/* profzen-server.h */

#ifndef PROFZEN_SERVER_H
#define PROFZEN_SERVER_H

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
#include "classroom.h"

extern volatile int force_exit;
extern struct lws_context *context;
extern char *resource_path;
extern char crl_path[1024];

extern Classroom classroom;

extern struct lws_protocols protocols[];

enum profzen_protocols {
	/* always first */
	PROTOCOL_HTTP = 0,

	PROTOCOL_WRITER,
	PROTOCOL_ANNOTATOR,

	/* always last */
	PROTOCOL_COUNT
};

struct per_session_data__http {
	lws_filefd_type	fd;
	int reason_bf;
	unsigned int client_finished:1;
};


struct per_session_data__profzen_writer {
	Writer writer;
};

struct per_session_data__profzen_annotator {
	Annotator annotator;
};


extern int
callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

extern int
callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

extern int
callback_profzen_writer(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

extern int
callback_profzen_annotator(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

#endif
