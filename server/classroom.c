/* classrooom.c */

#include "classroom.h"


Classroom Classroom_new (void)
{
	Classroom classroom = (Classroom) malloc(sizeof (struct Classroom));
	memset( (void*) classroom, 0, sizeof (struct Classroom ));

	classroom->Annotator = Annotator_new(classroom, NULL);
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
	Writer last;
	for( last = classroom->Writer; last->Next != NULL; last = last->Next );
	last->Next = Writer_new(classroom, last, wsi);
	return last->Next;
}


