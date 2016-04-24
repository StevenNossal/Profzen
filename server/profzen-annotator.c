
/* profzen-annotator.c */

#include "profzen-server.h"

int
callback_profzen_annotator(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	int n, m;
	
	struct per_session_data__profzen_annotator *pss = 
		(struct per_session_data__profzen_annotator *) user;

	switch (reason) {

		case LWS_CALLBACK_ESTABLISHED:
			lwsl_info("%s: LWS_CALLBACK_ESTABLISHED\n", __func__);
			break;

		case LWS_CALLBACK_PROTOCOL_DESTROY:
			lwsl_notice("%s: profzen writer protocol cleaning up\n", __func__);
			break;

		case LWS_CALLBACK_SERVER_WRITEABLE:
			m = strlen(text);
			n = lws_write(wsi, (unsigned char*) text, m, LWS_WRITE_TEXT);
			if ( n < 0 ) {
				lwsl_err("ERROR %d writing to profzen-annotator socket\n", n);
				return -1;
			}
			if ( n < m ) {
				lwsl_err("prozen-annotator partial write to %d vs %d\n", n, m);
			}
			break;		


		case LWS_CALLBACK_RECEIVE:
			lwsl_notice("%s: LWS_CALLBACK_RECEIVE\n", __func__);
			break;

		default:
			if ( NULL != pss ) pss = NULL;		/* Delete this line! */
			break;
		}

	return 0;
}
