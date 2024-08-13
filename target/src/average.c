
#include "circBufT.h"
#include "average.h"

#define BUFFER_SIZE 10


void initAverager(averager* averageStruct)
{
	initCircBuf(&(averageStruct->buffer), BUFFER_SIZE);
	averageStruct->sum = 0;
}

void storeData(averager* averageStruct, int32_t value)
{
	// Get value to be overwritten and minus it from sum
	circBuf_t* buffer_address = &(averageStruct->buffer); // Just getting address 
	averageStruct->sum -= buffer_address->data[buffer_address->windex];

	writeCircBuf(&(averageStruct->buffer), value);
}

// *******************************************************
// getAverageCircBuf: read all unread index to get current Average and return average
int32_t getAverage(averager* averageStruct)
{
	int32_t entry = 0;

	for (uint8_t i = 0; i < averageStruct->buffer.size; i++) 
	{
		if (readCircBuf(&(averageStruct->buffer), &entry))
		{
			averageStruct->sum += entry;
		}	
	}

	return averageStruct->sum / BUFFER_SIZE;
}