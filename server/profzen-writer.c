/* profzen-writer.c */

#include "profzen-server.h"

int
callback_profzen_writer(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	memset(&text, 0, sizeof text);
	
	struct per_session_data__profzen_writer *pss = 
		(struct per_session_data__profzen_writer *) user;

	switch (reason) {

		case LWS_CALLBACK_ESTABLISHED:
			lwsl_info("%s: LWS_CALLBACK_ESTABLISHED\n", __func__);
			break;

		case LWS_CALLBACK_PROTOCOL_DESTROY:
			lwsl_notice("%s: profzen writer protocol cleaning up\n", __func__);
			break;

		case LWS_CALLBACK_RECEIVE:
			lwsl_notice("%s: LWS_CALLBACK_RECEIVE\n", __func__);
			strncpy(text, in, len);
			lwsl_notice("pss.text:%s\n", text);
			lws_callback_on_writable_all_protocol(lws_get_context(wsi), &(protocols[PROTOCOL_ANNOTATOR]));
			break;
		
		
		default:
			if (NULL != pss) pss = NULL;  /* delete this line! */
			break;
		}

	return 0;
}
