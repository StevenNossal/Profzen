/* annotator.c */

#include "classroom.h"
#include "classroom_private.h"

//void
//GetThumbnailSocketData( Annotator annotator );

void
GetWriterList( Annotator annotator );

void
Annotator_ZeroSendBuffer( Annotator annotator );

void
GetWriterInnerHTML( Annotator annotator, char* payload );

AnnotatorWriter
AnnotatorWriter_New(Annotator annotator, Writer writer);

AnnotatorWriter
Annotator_AddAnnotatorWriter(Annotator annotator, Writer writer);


AnnotatorWriter
AnnotatorWriter_Find(Annotator annotator, Writer writer);

Annotator
Annotator_New (Classroom classroom, Annotator previous)
{
	lwsl_notice("%s:\n", __func__);
	Annotator annotator = (Annotator) malloc(sizeof (struct Annotator));
	memset(annotator, 0, sizeof( struct Annotator ));
	
	annotator->classroom = classroom;
	annotator->previous = previous;

	annotator->annotatorWriter = AnnotatorWriter_New( annotator, NULL ); 
	
	annotator->hasWrite = 0;
	annotator->sendBufferUserStart = annotator->sendBuffer + LWS_SEND_BUFFER_PRE_PADDING;
	annotator->sendBufferLength = 0;	

	return annotator;
}

void
Annotator_Free(Annotator annotator)
{
}

void
Annotator_ZeroSendBuffer( Annotator annotator )
{
  memset( annotator->sendBuffer, 0, sizeof( annotator->sendBuffer ));
  annotator->sendBufferLength = 0;  
}

void
Annotator_PutSocketReceiveData( Annotator annotator, char *in, size_t len){
  
  int nCommand;
  char* payload = NULL;

  if ( len > 1 )
  {
	sscanf(in, "%02d", &nCommand );
    payload = in + 2;	
  }

  lwsl_notice("%s, nCommand = %d", __func__, nCommand );

  if ( 1 == nCommand )
  {
	GetWriterList( annotator );
  }
  else if ( 2 == nCommand )
  {
	GetWriterInnerHTML( annotator, payload ); 
  }

}


int
Annotator_GetSocketWriteData( Annotator annotator, char **out, size_t* len)
{

	AnnotatorWriter aw = NULL;
	int hasWrite = annotator->hasWrite;

	if ( 1 == hasWrite ) {
		*out = annotator->sendBufferUserStart;
		*len = annotator->sendBufferLength;
		annotator->hasWrite = 0;
	} else {
		aw = annotator->annotatorWriter;
		for ( ; NULL != aw && 1 != aw->hasWrite; aw = aw->next );
		if ( NULL != aw ) {
			*out = aw->writer->docHTMLUserStart;
			*len = aw->writer->docHTMLLength;
			hasWrite = aw->hasWrite;
			aw->timeLastAnnotatorSend = time(NULL);
			aw->hasWrite = 0;
		}
	}

	return hasWrite;
}

void
Annotator_Update( Annotator annotator, Writer writer )
{
	AnnotatorWriter aw = AnnotatorWriter_Find( annotator, writer );
	if ( NULL == aw ) {
		aw = Annotator_AddAnnotatorWriter( annotator, writer );
	}

	aw->timeLastWriterChange = writer->timeLastChange;
	aw->hasWrite = 1;
}

		
void
GetWriterList( Annotator annotator )
{
  Classroom classroom = annotator->classroom;
  Writer writer = NULL;
  char* nextWrite = annotator->sendBufferUserStart;

  Annotator_ZeroSendBuffer( annotator );

  nextWrite += sprintf( nextWrite, "01[" );

  if ( NULL != classroom ) writer = classroom->writer->next;
  if ( NULL != writer ) {
	  for( ; writer != NULL; writer = writer->next ){
		nextWrite += sprintf( nextWrite, "%d,", writer->writerNumber );
	  }
	  nextWrite -= 1;
  }
  nextWrite += sprintf( nextWrite, "]");

  annotator->sendBufferLength = strlen( annotator->sendBufferUserStart );
  annotator->hasWrite = 1;
}

void
GetWriterInnerHTML( Annotator annotator, char* payload )
{
  lwsl_notice("%s payload=%s", __func__, payload );
  Classroom classroom = annotator->classroom;
  Writer writer = NULL;

  int writerNumber = 0;

  sscanf( payload, "%02d", &writerNumber );

  if ( NULL != classroom ) writer = classroom->writer->next;
  for ( ; writer != NULL && writer->writerNumber != writerNumber; writer = writer->next );
  if ( NULL != writer ) {
	Annotator_Update( annotator, writer );
  } 
}

AnnotatorWriter
Annotator_AddAnnotatorWriter( Annotator annotator, Writer writer )
{
  AnnotatorWriter aw = AnnotatorWriter_New( annotator, writer );
  AnnotatorWriter last = annotator->annotatorWriter;
  for( ; NULL != last->next; last = last->next );
  last->next = aw;
  return aw;
}


AnnotatorWriter 
AnnotatorWriter_New( Annotator annotator, Writer writer )
{
  AnnotatorWriter aw = malloc( sizeof( struct AnnotatorWriter ));
  memset( aw, 0, sizeof( struct AnnotatorWriter ));
  
  aw->annotator = annotator;
  aw->writer = writer;
  
  return aw;
}

AnnotatorWriter
AnnotatorWriter_Find( Annotator annotator, Writer writer )
{
  AnnotatorWriter aw = annotator->annotatorWriter; 
  for ( ; aw != NULL && aw->writer != writer; aw = aw-> next );
  return aw;
}

