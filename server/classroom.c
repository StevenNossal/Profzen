/* classrooom.c */

#include "classroom.h"

void Annotator_Update(Annotator annotator);
void Classroom_UpdateAnnotators(Classroom classroom);


Classroom Classroom_new (void)
{
	Classroom classroom = (Classroom) malloc(sizeof (struct Classroom));
	memset( (void*) classroom, 0, sizeof (struct Classroom ));

	classroom->nextWriter = 0;
	classroom->Annotator = Annotator_new(classroom, NULL, NULL);
	classroom->Writer = Writer_new(classroom, NULL, NULL);

	return classroom;
}

Annotator Annotator_new (Classroom classroom, Annotator previous, struct lws *wsi)
{
	Annotator annotator = (Annotator) malloc(sizeof (struct Annotator));
	memset(annotator, 0, sizeof( struct Annotator ));
	annotator->Classroom = classroom;
	annotator->Previous = previous;
	annotator->wsi = wsi;
	return annotator;
}

Writer Writer_new (Classroom classroom, Writer previous, struct lws *wsi)
{
	Writer writer = (Writer) malloc(sizeof (struct Writer));
	memset(writer, 0, sizeof( struct Writer));
	writer->Classroom = classroom;
	writer->Previous = previous;
	writer->wsi = wsi;
	writer->writerNumber = (classroom->nextWriter)++;
	writer->text = malloc( sizeof(char) * 4096 );
	writer->len = 0;

	return writer;
}



Annotator Classroom_AddAnnotator( Classroom classroom, struct lws *wsi )
{
	Annotator last;
	for(last = classroom->Annotator; last->Next != NULL; last = last->Next );
	last->Next = Annotator_new(classroom, last, wsi);
	return last->Next;
}

Writer Classroom_AddWriter( Classroom classroom, struct lws *wsi )
{
	lwsl_notice("%s:\n", __func__);
	
	Writer last;
	for( last = classroom->Writer; last->Next != NULL; last = last->Next );
	last->Next = Writer_new(classroom, last, wsi);
	return last->Next;
}


void Writer_SetText( Writer writer, char* text, size_t len )
{
	lwsl_notice("%s:\n", __func__);
	
	memset( writer->text, 0, sizeof(char) * 4096);
	memcpy( writer->text, text, len );
	writer->len = len;

	Classroom_UpdateAnnotators( writer->Classroom );	
}

char* Writer_GetText( Writer writer, size_t* len)
{
	*len = writer->len;
	return writer->text;
}

int Writer_GetWriterNumber( Writer writer )
{
	return writer->writerNumber;
}

void Classroom_UpdateAnnotators( Classroom classroom )
{
	lwsl_notice("%s:\n", __func__);
	Annotator annotator;
	for( annotator = classroom->Annotator->Next; annotator != NULL; annotator = annotator->Next )
	{
		Annotator_Update( annotator );
	}
}

void Annotator_Update( Annotator annotator )
{
	
	lwsl_notice("%s:&annotator->wsi = %d\n", __func__, &annotator->wsi);
	lws_callback_on_writable( annotator->wsi );
}	
