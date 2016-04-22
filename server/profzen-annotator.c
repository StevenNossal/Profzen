
/* profzen-annotator.c */

#include "profzen-server.h"

int
callback_profzen_annotator(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	struct per_session_data__profzen_annotator *pss = 
		(struct per_session_data__profzen_annotator *) user;

	switch (reason) {

		default:
			if ( NULL != pss ) pss = NULL;		/* Delete this line! */
			break;
		}

	return 0;
}
