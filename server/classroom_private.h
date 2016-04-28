/* classroom_private.h */

#ifndef CLASSROOM_PRIVATe_H
#define CLASSROOM_PRIVATE_H


typedef struct AnnatatorUpdate *AnnatatorUpdate;


struct Classroom
{
	Annotator	Annotator;
	Writer		Writer;

	int			nextWriter;
};

struct Writer
{
	Classroom	Classroom;
	Writer		Previous;
	Writer		Next;

	void		*socket;

	int			writerNumber;
	char*		text;
	size_t		len;
};

struct Annotator
{
	Classroom	Classroom;
	Annotator	Previous;
	Annotator	Next;

	void		*socket;

	AnnatatorUpdate		updates;

};

struct AnnatatorUpdate
{
	Writer				writer;
	
	char*				text;
	size_t				len;

	AnnatatorUpdate		previous;
	AnnatatorUpdate		next;
};


Annotator
Annotator_New (Classroom classroom, Annotator previous, struct lws *wsi);

void
Annotator_Free(Annotator annotator);

AnnatatorUpdate
AnnatatorUpdate_New( AnnatatorUpdate previous, Writer writer );

void
AnnotatorUpdate_Free( AnnotatorUpdate annotatorUpdate );

Writer
Writer_New(Classroom classroom, Writer previous, struct lws *wsi);

void
Writer_Free(Writer writer);

#endif
