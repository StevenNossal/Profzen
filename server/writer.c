/* writer.c */

#include "classroom.h"
#include "classroom_private.h"

Writer
Writer_New (Classroom classroom, Writer previous)
{
	Writer writer = (Writer) malloc(sizeof (struct Writer));
	memset(writer, 0, sizeof( struct Writer));
	writer->classroom = classroom;
	writer->previous = previous;
	writer->writerNumber = (classroom->nextWriter)++;

	writer->receiveBuffer = malloc( sizeof(char) * 4096 );
	writer->len = 0;

	return writer;
}

void
Writer_Free( Writer writer )
{

}


void
Writer_PutSocketReceiveData( Writer writer, char *in, size_t len)
{	
	lwsl_debug("%s:\n", __func__);
	
	memset( writer->receiveBuffer, 0, sizeof(char) * 4096);
	memcpy( writer->receiveBuffer, in, sizeof(char) * len );
	writer->len = len;

	Classroom_UpdateAnnotators( writer->classroom, writer );
}	
