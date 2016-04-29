/* classroom_private.h */

#ifndef CLASSROOM_PRIVATE_H
#define CLASSROOM_PRIVATE_H


struct Classroom
{
	Annotator	annotator;
	Writer		writer;

	int			nextWriter;
};

struct Writer
{
	Classroom	classroom;
	Writer		previous;
	Writer		next;

	int			writerNumber;
	char		*receiveBuffer;
	size_t		len;
};

struct Annotator
{
	Classroom	classroom;
	Annotator	previous;
	Annotator	next;

	int			hasWrite;

	char		*sendBuffer;
	size_t		len; 
	
};

void
Classroom_UpdateAnnotators(Classroom classroom, Writer writer);

Annotator
Classroom_GetNextAnnotator( Classroom classroom, Annotator annotator );

Annotator
Annotator_New (Classroom classroom, Annotator previous);

void
Annotator_Free(Annotator annotator);

void
Annotator_Update( Annotator annotator, Writer writer );

Writer
Writer_New(Classroom classroom, Writer previous);

void
Writer_Free(Writer writer);

#endif
