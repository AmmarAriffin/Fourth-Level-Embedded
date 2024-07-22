// *******************************************************
// 
// circBufT.c
//
// Support for a circular buffer of uint32_t values on the 
//  Tiva processor.
// P.J. Bones UCECE
// Last modified:  8.3.2017
// 
// *******************************************************

#include <stdint.h>
#include "stdlib.h"
#include "circBufT.h"

uint32_t MAX_SIZE = 100;
// *******************************************************
// initCircBuf: Initialise the circBuf instance. Reset both indices to
// the start of the buffer.  Dynamically allocate and clear the the 
// memory and return a pointer for the data.  Return NULL if 
// allocation fails.
uint32_t *
initCircBuf (circBuf_t *buffer, uint32_t size)
{
	if (size <= 0 || size > MAX_SIZE)  {
		return NULL;
	} else {
		buffer->windex = 0;
		buffer->rindex = 0;
		buffer->size = size;
		buffer->count = 0;
		buffer->data = 
			(uint32_t *) calloc (size, sizeof(uint32_t));
		return buffer->data;
	}	
}
   // Note use of calloc() to clear contents.

// *******************************************************
// writeCircBuf: insert entry at the current windex location,
// advance windex, modulo (buffer size).
void
writeCircBuf (circBuf_t *buffer, int32_t entry)
{	
	//checks to see if writing to windex will result in an overwrite
	if (buffer->count == buffer->size) {
		return;
	} else {
		buffer->data[buffer->windex] = entry;
		buffer->windex++;
		buffer->count++;
		if (buffer->windex >= buffer->size)
		buffer->windex = 0;
	}
}

// *******************************************************
// readCircBuf: return entry at the current rindex location,
// advance rindex, modulo (buffer size). The function deos not check
// if reading has advanced ahead of writing.
uint8_t
readCircBuf (circBuf_t *buffer, uint32_t *entry)
{
	//if write wraps around and read hasnt been called in a full loop then the buffer will be full but it will thinks its empty
	if (buffer->rindex == buffer->windex && buffer->count == 0) {
		//buffer empty
		return 0;
	} else {
		*entry = buffer->data[buffer->rindex];
		buffer->rindex++;
		buffer->count--;
		if (buffer->rindex >= buffer->size)
		buffer->rindex = 0;
		return 1;
	}
}

// *******************************************************
// freeCircBuf: Releases the memory allocated to the buffer data,
// sets pointer to NULL and other fields to 0. The buffer can
// re-initialised by another call to initCircBuf().
void
freeCircBuf (circBuf_t * buffer)
{
	buffer->windex = 0;
	buffer->rindex = 0;
	buffer->size = 0;
	free (buffer->data);
	buffer->data = NULL;
}

