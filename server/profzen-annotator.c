
/* profzen-annotator.c */

#include "profzen-server.h"

int
callback_profzen_annotator(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	int m, n;
	unsigned char *socket_write_data;
	Annotator annotator;
	
	struct per_session_data__profzen_annotator *pss = 
		(struct per_session_data__profzen_annotator *) user;

	
	lwsl_notice("%s, reason = %d\n", __func__, reason);
	
	switch (reason) {

		case LWS_CALLBACK_ESTABLISHED:
			lwsl_notice("%s: LWS_CALLBACK_ESTABLISHED\n", __func__);
			lwsl_notice("wsi = %d\n", wsi);
			pss->annotator = Classroom_AddAnnotator( classroom );
			break;

		case LWS_CALLBACK_PROTOCOL_DESTROY:
			lwsl_notice("%s: profzen writer protocol cleaning up\n", __func__);
			break;

		case LWS_CALLBACK_SERVER_WRITEABLE:
			lwsl_notice("%s: LWS_CALLBACK_WRITEABLE\n", __func__);
			annotator = pss->annotator;
			
			if ( 1 == Annotator_GetSocketWriteData( annotator, (char**) &socket_write_data, (size_t*) &m )) {
				n = lws_write(wsi, socket_write_data,  m, LWS_WRITE_TEXT);
				lwsl_notice("Writing %s\n", socket_write_data);
				if ( n < 0 ) {
					lwsl_err("ERROR %d writing to profzen-annotator socket\n", n);
				return -1;
				}
				if ( n < m ) {
					lwsl_err("prozen-annotator partial write to %d vs %d\n", n, m);
				}
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
