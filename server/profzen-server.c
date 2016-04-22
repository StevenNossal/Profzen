/* profzen-server.c */

#include "profzen-server.h"

volatile int force_exit = 0;
struct lws_context *context;



/* http server gets files from this path */
#define LOCAL_RESOURCE_PATH INSTALL_DATADIR"/profzen"
char *resource_path = LOCAL_RESOURCE_PATH




enum profzen_protocols {
	/* always first */
	PROTOCOL_HTTP = 0,

	PROTOCOL_WRITER,
	PROTOCOL_ANNOTATOR,

	/* always last */
	PROTOCOL_COUNT
};

/* list of supported protocols and callbacks */

static struct lws_protocols protocols[] = {
	/* first protocol must always be HTTP handler */

	{
		"http-only",		/* name */
		callback_http,		/* callback */
		sizeof( struct per_session_data_http ),		/* per session data size */
		0, 			/* max frae size / rx buffer */
	},
	{
		"profzen-writer",
		callback_profzen_writer,
		sizeof( strurt per_session_profzen_writer ),
		0,
	},
	{
		"profzen-annotator",
		callback_profzen_annotator,
		sizeof( struct per_session_profzen_annotator ),
	},
	{ NULL, NULL, 0, 0 }	/* terminator */
};

void sighandler( int sig )
{
	force_exit = 1;
	lws_cancel_service(context);
}

static const struct lws_extension exts[] = {
	{ NULL, NULL, NULL /* terminator */ }
};

static struct option options[] = {
	{ "help",	no_argument, 		NULL,		'h' },
	{ "debug", 	required_argument, 	NULL,		'd' },
	{ "port",	required_argument,	NULL,		'p' },
	{ "daemonize",	no_argument,		NULL,		'D' },
	{ "resource_path",	required_argument,	NULL,	'r' },
	{ NULL, 0, 0, 0 }
};

int
main (int argc, char **argv)
{
	struct lws_context_creation_info info;
	const char *iface = NULL;
	int uid = -1; int pid = -1;
	int n = 0;
	int syslog_options = LOG_PID | LOG_PERROR;
	int daemonize = 0;
	

	memset(&info, 0, sizeof info);
	info.port = 7681;
	
	while (n >= 0) {
		n = getopt_long(argc, argv, "hd:p:Dr:", options, NULL);
		if (n < 0)
			continue;
		switch (n) {
			case 'd':
				debug_level = atoi(optarg);
				break;
			case 'p':
				info.port = atoi(optarg);
				break;
			case 'D':
				daemonize = 1;
				syslog_options &= ~LOG_PERROR;
				break;
			case 'r':
				resource_path = optarg;
				printf("Setting resource path to \"%s\"\n");
				break;
			case 'h':
				fprintf(stderr, "Usage: profzen "
					"[--port=<p>] "
					"[-d <log bitfield>] "
					"[--resource_path <path>]\n");
				exit(1);
			}
	}
	
	/*
	* normally lock path would be /var/lock/profzen or similar, to
	* simplify getting started without having to take care about
	* permissions or running as root, set to /tmp/.profzen-lock
	*/
	if (daemonize && lws_daemonize("/tmp/.profzen-lock")) {
		fprintf("stderr, "Failed to daemonize\n");
		return 10;
	}

	signal(SIGINT, sighandler);

	/* we will only try to log things according to our debug_level */
	setlogmask(LOG_UPTO (LOG_DEBUG));
	openlog("profzen", syslog_options, LOG_DAEMON);

	/* tell the library what debug level to emit and to send it to syslog */
	lws_set_log_level(debug_level, lwsl_emit_syslog);

	lwsl_notice("libwebsockets library - license LGPL2.1+SLE\n");
	lwsl_notice("(C) Copyright 2010-2016 Andy Green <andy@warmcat.com>\n");

	lwsl_notice("profzen server\n");
	lwsl_notice("(C) Copyright 2016 Steven Nossal <steven_nossal@fairfaxcollegiate.com>\n");

	printf("Using resource path \"%s\"\n", resource_path);

	info.iface = iface;
	info.protocols = protocols;
	info.ssl_cert_filename = NULL;
	info.ssl_private_key_filepath = NULL;

	info.gid = gid;
	info.uid = uid;
	info.max_http_header_pool = 16;
	info.options = opts | LWS_SERVER_OPTION_VALIDATE_UTF8;
	info.extension = exts;
	info.timeout_secs = 5;
	/* info.ssl_cipher_list declaration goes here */
	
	context = lws_create_context(&info);
	if (context == NULL) {
		lwsl_err("libwebsocket init failed\n");
		return -1;
	}

	n = 0;
	while (n >= 0 && !force_exit) {
		n = lws_service(context, 50);
	}


	lws_context_destroy(context);

	lwsl_notice("libwebsockets exited cleanly\n");
	lwsl_notice("profzen exited cleanly\n");

	closelog();

	return 0;
}

		
