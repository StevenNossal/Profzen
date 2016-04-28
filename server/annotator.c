/* annotator.c */

#include "classroom.h"
#include "classroom_private.h"

Annotator
Annotator_New (Classroom classroom, Annotator previous, void* socket)
{
	Annotator annotator = (Annotator) malloc(sizeof (struct Annotator));
	memset(annotator, 0, sizeof( struct Annotator ));
	annotator->Classroom = classroom;
	annotator->Previous = previous;
	annotator->socket = socket;
	
	annotator->updates = AnnatatorUpdate_New( NULL, NULL );
	
	return annotator;
}

void
Annotator_Free(Annotator annotator)
{
}

void*
Annotator_GetSocket( Annotator annotator )
{
	return annotator->socket;
}

void
Annotator_AddToUpdateList( Annotator annotator, Writer writer )
{
	AnnatatorUpdate last;
	for ( last = annotator->updates->next; last != NULL; last = last->next );

	last->next = AnnatatorUpdate_New( last, writer );
}

AnnatatorUpdate
AnnatatorUpdate_New( AnnatatorUpdate previous, Writer writer )
{
	AnnotatorUpdate au = malloc( sizeof (struct AnnatatorUpdate));
	au->writer = writer;
	au->previous = NULL;
	au->next = NULL;
	return au;
}

void
AnnatatorUpdate_Free( AnnotatorUpdate au )
{
}

char*
AnnatatorUpdate_GetText( AnnatatorUpdate au, size_t* len )
{
	if ( au->text == NULL )
	{
		au->len = au->writer->len + 2;
		au->text = malloc( sizeof( char ) * au->len );
		memset( au->text, 0, sizeof( char ) * au->len );
		snprintf( au->text, au->len, "%02d%s", au->writer->writerNumber, au->writer->text );
	}

	*len = au->len;
	return au->text;
}


void
AnnatatorUpdate_Remove( AnnatatorUpdate annatatorUpdate )
{
	if ( annatatorUpdate->previous != NULL ) {
		annatatorUpdate->previous = annatatorUpdate->next; }

	if ( annatatorUpdate->text != NULL ) {
		free( annatatorUpdate->text );
	}

	free ( annatatorUpdate );
}
		
