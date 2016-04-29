/* annotator.c */

#include "classroom.h"
#include "classroom_private.h"

Annotator
Annotator_New (Classroom classroom, Annotator previous)
{
	lwsl_notice("%s:\n", __func__);
	Annotator annotator = (Annotator) malloc(sizeof (struct Annotator));
	memset(annotator, 0, sizeof( struct Annotator ));
	
	annotator->classroom = classroom;
	annotator->previous = previous;
	
	annotator->hasWrite = 0;
	annotator->sendBuffer = (char*) malloc( sizeof( char ) * 4096 );
	annotator->len = 0;	


	return annotator;
}

void
Annotator_Free(Annotator annotator)
{
}


int
Annotator_GetSocketWriteData( Annotator annotator, char **out, size_t* len)
{
	int hasWrite = annotator->hasWrite;

	if ( 1 == hasWrite )
	{
		*out = annotator->sendBuffer + LWS_SEND_BUFFER_PRE_PADDING;
		*len = annotator->len;
		annotator->hasWrite = 0;
	}

	return hasWrite;
}

void
Annotator_Update( Annotator annotator, Writer writer )
{
	memset( annotator->sendBuffer, 0, sizeof(char) * 4096);
	snprintf( annotator->sendBuffer + LWS_SEND_BUFFER_PRE_PADDING, writer->len + 3, "%02d%s",
			writer->writerNumber, writer->receiveBuffer );
	annotator->len = writer->len + 2;
	annotator->hasWrite = 1;
}
	
	
