/* profzen-writer.c */

#include "profzen-server.h"


int
callback_profzen_writer(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	
	struct per_session_data__profzen_writer *pss = 
		(struct per_session_data__profzen_writer *) user;

	switch (reason) {

		case LWS_CALLBACK_ESTABLISHED:
			lwsl_notice("%s: LWS_CALLBACK_ESTABLISHED\n", __func__);
		
			pss->writer = Classroom_AddWriter( classroom );

			break;

		case LWS_CALLBACK_PROTOCOL_DESTROY:
			lwsl_notice("%s: profzen writer protocol cleaning up\n", __func__);
			break;

		case LWS_CALLBACK_RECEIVE:
			lwsl_notice("%s: LWS_CALLBACK_RECEIVE\n", __func__);
			lwsl_notice("in:%s\n", in);
			
			Writer writer = pss->writer;
			Writer_PutSocketReceiveData( writer, in, len);

			lws_callback_on_writable_all_protocol(context, &protocols[PROTOCOL_ANNOTATOR]);

			break;
		
		default:
			break;
		}

	return 0;
}


