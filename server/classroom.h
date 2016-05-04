/* classroom.h */

#ifndef CLASSROOM_H
#define CLASSROOM_H

#include "libwebsockets.h"

typedef struct Classroom *Classroom;
typedef struct Writer *Writer;
typedef struct Annotator *Annotator;

Classroom
Classroom_New (void);

void
Classroom_Free(Classroom classroom);

Writer
Classroom_AddWriter( Classroom classroom );

Annotator
Classroom_AddAnnotator( Classroom classroom );

void
Writer_PutSocketReceiveData( Writer writer, char *in, size_t len);

/*
 * Annotator Socket API
 * Command O1: Return thumbnails
 * Thumbnails: 0000 --- Number of Thumbnail followed by thumbnail
 * Thumbnail: 0000 -- Thumbnail number
 *			  000000 -- Data length
 *			  Data
 *
 */

void
Annotator_PutSocketReceiveData( Annotator annotator, char *in, size_t len);

int
Annotator_GetSocketWriteData( Annotator annotator, char **out, size_t* len);

#endif
