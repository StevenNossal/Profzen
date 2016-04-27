/* classroom.h */

#ifndef CLASSROOM_H
#define CLASSROOM_H

#include "libwebsockets.h"

typedef struct Classroom *Classroom;
typedef struct Writer *Writer;
typedef struct Annotator *Annotator;

struct Classroom
{
	Annotator	Annotator;
	Writer		Writer;

	int			nextWriter;
};

struct Annotator
{
	Classroom	Classroom;
	Annotator	Previous;
	Annotator	Next;

	struct lws *wsi;
};

struct Writer
{
	Classroom	Classroom;
	Writer		Previous;
	Writer		Next;

	struct lws *wsi;

	int			writerNumber;
	char*		text;
	size_t		len;
};

Classroom Classroom_new (void);
Annotator Annotator_new (Classroom classroom, Annotator previous, struct lws *wsi);
Writer Writer_new(Classroom classroom, Writer previous, struct lws *wsi);

Annotator Classroom_AddAnnotator( Classroom classroom, struct lws *wsi );
Writer Classroom_AddWriter( Classroom classroom, struct lws *wsi );

void Writer_SetText( Writer writer, char* text, size_t len );
char* Writer_GetText( Writer writer, size_t* len);

int Writer_GetWriterNumber( Writer writer );
#endif
