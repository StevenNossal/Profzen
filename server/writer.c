/* writer.c */

#include "classroom.h"
#include "classroom_private.h"

void
set_docHTML( Writer writer, char* in, size_t len );

void
mark_change_time( Writer writer );




Writer
Writer_New (Classroom classroom, Writer previous)
{
	Writer writer = (Writer) malloc(sizeof (struct Writer));
	memset(writer, 0, sizeof( struct Writer));
	writer->classroom = classroom;
	writer->previous = previous;

	writer->writerNumber = (classroom->nextWriter)++;
	writer->timeLastChange = time(NULL);

	writer->receiveBufferLength = 0;

	writer->docHTMLBuffer = NULL;
	writer->docHTMLUserStart = NULL;
	writer->docHTMLLength = 0;

	return writer;
}

void
Writer_Free( Writer writer )
{

}


void
Writer_PutSocketReceiveData( Writer writer, char *in, size_t len)
{	
	assert( writer );
	assert( writer->writerNumber != 0);
	
	// TODO: Implement Writer Commands
	// 01 -- Reserved for Login
	// 02 -- Set docHTML
	//
	
	int nCommand = 2;

	lwsl_debug("%s:\n", __func__);
	
	memset( writer->receiveBuffer, 0, sizeof(writer->receiveBuffer));
	memcpy( writer->receiveBuffer, in, sizeof(char) * len );
	writer->receiveBufferLength = len;

	if ( 1 == nCommand ) {
		// Login
	}
	else if ( 2 == nCommand ) {
	  set_docHTML( writer, in, len );
	}

}	

void
set_docHTML( Writer writer, char* in, size_t len )
{
	assert( writer );
	assert( writer->writerNumber != 0 );

	lwsl_notice("%s, writer = %d, writerNumber = %d", __func__, writer, writer->writerNumber );
    lwsl_notice("%s, in: %s", __func__, in);	

	size_t bufSize = LWS_SEND_BUFFER_PRE_PADDING + sizeof(char) * len + 5;
	
	if ( NULL != writer->docHTMLBuffer ){
		free( writer->docHTMLBuffer);
		writer->docHTMLBuffer = NULL;
		writer->docHTMLUserStart = NULL;
		writer->docHTMLLength = 0;
	}

	writer->docHTMLBuffer = malloc ( bufSize );
	writer->docHTMLUserStart = writer->docHTMLBuffer + LWS_SEND_BUFFER_PRE_PADDING;

	memset( writer->docHTMLBuffer, 0, bufSize );
	sprintf(writer->docHTMLUserStart, "02%02d%s", writer->writerNumber, in);
	writer->docHTMLLength = strlen( writer->docHTMLUserStart );

	writer->timeLastChange = time(NULL);

	Classroom_UpdateAnnotators( writer->classroom, writer );
}

