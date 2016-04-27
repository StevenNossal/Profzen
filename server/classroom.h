/* classroom.h */

#ifndef CLASSROOM_H
#define CLASSROOM_H

#include "profzen-server.h"

typedef struct Classroom *Classroom;
typedef struct Writer *Writer;
typedef struct Annotator *Annotator;

struct Classroom
{
	Annotator	Annotator;
	Writer		Writer;
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
};

Classroom Classroom_new (void);
Annotator Annotator_new (Classroom classroom, Annotator previous);
Writer Writer_new(Classroom classroom, Writer previous, struct lws *wsi);

Annotator Classroom_AddAnnotator( Classroom classroom, struct lws *wsi );
Writer Classroom_AddWriter( Classroom classroom, struct lws *wsi );


#endif
