
/* profzen-annotator.c */

#include "profzen-server.h"

int
callback_profzen_annotator(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	int i, n, m;
	
	struct per_session_data__profzen_annotator *pss = 
		(struct per_session_data__profzen_annotator *) user;

	
	lwsl_notice("%s, reason = %d\n", __func__, reason);
	
	switch (reason) {

		case LWS_CALLBACK_ESTABLISHED:
			lwsl_notice("%s: LWS_CALLBACK_ESTABLISHED\n", __func__);
			pss->annotator = Classroom_AddAnnotator( classroom, wsi );
			break;

		case LWS_CALLBACK_PROTOCOL_DESTROY:
			lwsl_notice("%s: profzen writer protocol cleaning up\n", __func__);
			break;

		case LWS_CALLBACK_SERVER_WRITEABLE:
			for ( i = 1; i < MAX_WRITERS + 1; i++)
			{
				struct per_session_data__profzen_writer *writer = writers[i].pss;
				if ( NULL != writer && writers[i].isDirty == 1 )
				{
					char textout[4098];
					memset( &textout, 0, sizeof(textout) );
					snprintf(textout, 4098, "%02d%s", writer->writerNumber, writer->text );
					m = strlen(textout);
					n = lws_write(wsi, (unsigned char*) textout, m, LWS_WRITE_TEXT);
					lwsl_notice("Writing %s\n", textout);
					if ( n < 0 ) {
						lwsl_err("ERROR %d writing to profzen-annotator socket\n", n);
						return -1;
					}
					if ( n < m ) {
						lwsl_err("prozen-annotator partial write to %d vs %d\n", n, m);
					}
//					writers[i].isDirty = 0;
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
