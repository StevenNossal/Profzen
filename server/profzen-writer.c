/* profzen-writer.c */

#include "profzen-server.h"

static int nextWriter = 0;


int
callback_profzen_writer(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	memset(&text, 0, sizeof text);
	
	struct per_session_data__profzen_writer *pss = 
		(struct per_session_data__profzen_writer *) user;

	switch (reason) {

		case LWS_CALLBACK_ESTABLISHED:
			lwsl_notice("%s: LWS_CALLBACK_ESTABLISHED\n", __func__);
			pss->writerNumber = ++nextWriter;
			memset(pss->text, 0, sizeof pss->text);
			writers[pss->writerNumber].pss = pss;
		
			pss->writer = Classroom_AddWriter( classroom, wsi );

			break;

		case LWS_CALLBACK_PROTOCOL_DESTROY:
			lwsl_notice("%s: profzen writer protocol cleaning up\n", __func__);
			break;

		case LWS_CALLBACK_RECEIVE:
			lwsl_notice("%s: LWS_CALLBACK_RECEIVE\n", __func__);
			if ( len > 4096 ) len = 4096;
			memset(pss->text, 0, sizeof pss->text);
			strncpy(pss->text, in, len);
			writers[pss->writerNumber].isDirty = 1;

			Writer writer = pss->writer;
			Writer_SetText( writer, in, len );


			lwsl_notice("pss.text:%s\n", pss->text);
//			lws_callback_on_writable_all_protocol(context, &protocols[PROTOCOL_ANNOTATOR]);
			break;
		
		default:
			break;
		}

	return 0;
}
