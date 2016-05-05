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
	lwsl_notice("%s, annotator->hasWrite = %d", __func__, annotator->hasWrite);

	AnnotatorWriter aw = NULL;
	int hasWrite = annotator->hasWrite;

	if ( 1 == hasWrite ) {
		lwsl_notice("%si: sending from annotator->sendBuffer", __func__);
		*out = annotator->sendBufferUserStart;
		*len = annotator->sendBufferLength;
		annotator->hasWrite = 0;
	} else {
		lwsl_notice("%s, sending from annotator->annotatorWriter...", __func__);
		aw = annotator->annotatorWriter;
		for ( ; NULL != aw && 0 == aw->hasWrite; aw = aw->next );
		lwsl_notice("%s, aw = %d", __func__, aw);
		if ( NULL != aw ) lwsl_notice("%s, aw->writer = %d", __func__, aw->writer);
		if ( NULL != aw && NULL != aw->writer) {
			lwsl_notice("%s: sending from annotator->annotatorWriter, writerNumber = %d",
				   	__func__, aw->writer->writerNumber);
			*out = aw->writer->docHTMLUserStart;
			*len = aw->writer->docHTMLLength;
			hasWrite = aw->hasWrite;
			aw->timeLastAnnotatorSend = time(NULL);
			aw->hasWrite = 0;
		}
	}

	lwsl_notice("Leaving %s, hasWrite = %d", __func__, hasWrite );
	
	return hasWrite;
}

void
Annotator_Update( Annotator annotator, Writer writer )
{
	assert( annotator );
	assert( writer );
	
	lwsl_notice("%s, annotator = %d, writer = %d", __func__, annotator, writer ); 
	lwsl_notice("%s, writer->writerNumber = %d", __func__, writer->writerNumber );

	AnnotatorWriter aw = AnnotatorWriter_Find( annotator, writer );
	if ( NULL == aw ) {
		aw = Annotator_AddAnnotatorWriter( annotator, writer );
	}

	assert( aw );
	assert( NULL != aw->writer );
	assert( 0 != aw->writer->writerNumber );

	lwsl_notice("%s, updating writerNumber = %d", __func__, aw->writer->writerNumber );
	if ( NULL != writer->docHTMLBuffer ) {
	  aw->timeLastWriterChange = writer->timeLastChange;
	  aw->hasWrite = 1;
	}
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
  lwsl_notice("%s, annotator = %d, writer = %d", __func__, annotator, writer );
  AnnotatorWriter aw = AnnotatorWriter_New( annotator, writer );
  AnnotatorWriter last = annotator->annotatorWriter;
  for( ; NULL != last->next; last = last->next );
  last->next = aw;

  lwsl_notice("%s, returning aw = %d", __func__, aw );
  return aw;
}


AnnotatorWriter 
AnnotatorWriter_New( Annotator annotator, Writer writer )
{
  lwsl_notice("%s, annotator = %d, writer = %d", __func__, annotator, writer );
  AnnotatorWriter aw = malloc( sizeof( struct AnnotatorWriter ));
  memset( aw, 0, sizeof( struct AnnotatorWriter ));
  
  aw->annotator = annotator;
  aw->writer = writer;
  
  lwsl_notice("%s, returning aw = %d", __func__, aw );
  return aw;
}

AnnotatorWriter
AnnotatorWriter_Find( Annotator annotator, Writer writer )
{
  lwsl_notice("%s, annotator = %d, writer = %d", __func__, annotator, writer );
  AnnotatorWriter aw = annotator->annotatorWriter; 
  for ( ; aw != NULL && aw->writer != writer; aw = aw-> next );
  
  lwsl_notice("%s, returning aw = %d", __func__, aw );
  return aw;
}

