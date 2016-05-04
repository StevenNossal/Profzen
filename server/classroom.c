/* classrooom.c */

#include "classroom.h"
#include "classroom_private.h"


Classroom
Classroom_New (void)
{
	Classroom classroom = (Classroom) malloc(sizeof (struct Classroom));
	memset( (void*) classroom, 0, sizeof (struct Classroom ));

	classroom->nextWriter = 0;
	classroom->annotator = Annotator_New(classroom, NULL);
	classroom->writer = Writer_New(classroom, NULL);

	return classroom;
}

void
Classroom_Free(Classroom classroom)
{
	lwsl_debug("%s:\n", __func__);
}

Annotator
Classroom_AddAnnotator( Classroom classroom )
{
	lwsl_debug("%s:\n", __func__);

	Annotator new = NULL;
	Annotator last = classroom->annotator;
	Writer writer = NULL;

	for(; NULL != last->next; last = last->next );
	new = Annotator_New(classroom, last);
	last->next = new;

	for( writer = classroom->writer->next; NULL != writer; writer = writer->next ) {
		Annotator_Update( new, writer );
	}

	return new;
}

Annotator
Classroom_GetNextAnnotator( Classroom classroom, Annotator annotator )
{
	lwsl_debug("%s:\n", __func__);
	if (NULL == annotator) annotator = classroom->annotator;
	return annotator->next;
}

Writer
Classroom_AddWriter( Classroom classroom)
{
	lwsl_debug("%s:\n", __func__);
	
	Writer last;
	for( last = classroom->writer; last->next != NULL; last = last->next );
	last->next = Writer_New(classroom, last);
	return last->next;
}

void
Classroom_UpdateAnnotators(Classroom classroom, Writer writer)
{
	lwsl_notice("%s:\n", __func__);
	Annotator annotator;
	for( annotator = classroom->annotator->next; annotator != NULL; annotator = annotator->next )
	{
		Annotator_Update(annotator, writer);
	}	
}
		
