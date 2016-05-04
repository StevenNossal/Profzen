/* classroom_private.h */

#ifndef CLASSROOM_PRIVATE_H
#define CLASSROOM_PRIVATE_H

#include <assert.h>

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
	time_t		timeLastChange;

	char		receiveBuffer[4096];
	size_t		receiveBufferLength;

	char		*docHTMLBuffer;
	char		*docHTMLUserStart;
	size_t		docHTMLLength;
};

typedef struct AnnotatorWriter	*AnnotatorWriter;

struct Annotator
{
	Classroom	classroom;
	Annotator	previous;
	Annotator	next;
	AnnotatorWriter	  annotatorWriter;

	int			hasWrite;

	char    	sendBuffer[4096];
	char		*sendBufferUserStart;
	size_t		sendBufferLength; 
	
};

struct AnnotatorWriter
{
	Annotator		  annotator;
	Writer			  writer;
	AnnotatorWriter	  next;

	time_t			  timeLastWriterChange;
	time_t			  timeLastAnnotatorSend;

	int				  hasWrite;

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
