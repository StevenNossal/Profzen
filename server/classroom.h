/* classroom.h */

#ifndef CLASSROOM_H
#define CLASSROOM_H

typedef struct Classroom *Classroom;
typedef struct Writer *Writer;
typedef struct Annotator *Annotator;

Classroom
Classroom_New (void);

void
Classroom_Free(Classroom classroom);

Writer
Classroom_AddWriter( Classroom classroom, void *socket );

Annotator
Classroom_AddAnnotator( Classroom classroom, void *socket );

Annotator
Classroom_GetNextAnnotator( Classroom classroom, Annotator annotator );

void
Writer_SetText( Writer writer, char* text, size_t len );

void*
Annotator_GetSocket( Annotator annotator );

char*
Annotator_GetSocketWriteData( Annotator annotator );

#endif
