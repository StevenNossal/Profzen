/* classrooom.c */

#include "classroom.h"
#include "classroom_private.h"


Classroom
Classroom_New (void)
{
	Classroom classroom = (Classroom) malloc(sizeof (struct Classroom));
	memset( (void*) classroom, 0, sizeof (struct Classroom ));

	classroom->nextWriter = 0;
	classroom->Annotator = Annotator_new(classroom, NULL, NULL);
	classroom->Writer = Writer_new(classroom, NULL, NULL);

	return classroom;
}

void
Classroom_Free(Classroom classroom)
{
	
}

Annotator
Classroom_AddAnnotator( Classroom classroom, void *socket )
{
	Annotator last;
	for(last = classroom->Annotator; last->Next != NULL; last = last->Next );
	last->Next = Annotator_new(classroom, last, socket);
	return last->Next;
}

Writer
Classroom_AddWriter( Classroom classroom, void *socket )
{
	lwsl_notice("%s:\n", __func__);
	
	Writer last;
	for( last = classroom->Writer; last->Next != NULL; last = last->Next );
	last->Next = Writer_new(classroom, last, socket);
	return last->Next;
}


		
