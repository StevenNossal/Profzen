/* writer.c */

#include "classroom.h"
#include "classroom_private.h"

Writer
Writer_New (Classroom classroom, Writer previous, void* socket)
{
	Writer writer = (Writer) malloc(sizeof (struct Writer));
	memset(writer, 0, sizeof( struct Writer));
	writer->Classroom = classroom;
	writer->Previous = previous;
	writer->socket = socket;
	writer->writerNumber = (classroom->nextWriter)++;
	writer->text = malloc( sizeof(char) * 4096 );
	writer->len = 0;

	return writer;
}

void
Writer_Free( Writer writer )
{

}

Writer
Classroom_AddWriter( Classroom classroom, struct lws *wsi )
{
	lwsl_notice("%s:\n", __func__);
	
	Writer last;
	for( last = classroom->Writer; last->Next != NULL; last = last->Next );
	last->Next = Writer_new(classroom, last, wsi);
	return last->Next;
}

void
Writer_SetText( Writer writer, char* text, size_t len )
{
	lwsl_notice("%s:\n", __func__);
	
	memset( writer->text, 0, sizeof(char) * 4096);
	memcpy( writer->text, text, len );
	writer->len = len;

	Classroom_UpdateAnnotators( writer->Classroom );	
}
