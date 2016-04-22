/* profzen-writer.c */

#include "profzen-server.h"

int
callback_profzen_writer(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	struct per_session_data__profzen_writer *pss = 
		(struct per_session_data__profzen_writer *) user;

	switch (reason) {

		default:
			if (NULL != pss) pss = NULL;  /* delete this line! */
			break;
		}

	return 0;
}
