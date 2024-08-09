
#include "circBufT.h"
#include "average.h"

#define BUFFER_SIZE 10


void initAverager(averager* averageStruct)
{
	initCircBuf(&(averageStruct->buffer), BUFFER_SIZE);
}

void storeData(averager* averageStruct, uint32_t value)
{
	writeCircBuf(&(averageStruct->buffer), value);
}

// *******************************************************
// getAverageCircBuf: read all unread index to get current Average and return average
uint32_t getAverage(averager* averageStruct)
{
	uint32_t sum = 0;
	uint32_t entry = 0;
	uint8_t numValues = averageStruct->buffer.count;

	for (uint8_t i = 0; i < averageStruct->buffer.size; i++) 
	{
		if (readCircBuf(&(averageStruct->buffer), &entry))
		{
			sum = sum + entry;
		}	
	}
	if (numValues == 0) {
		return sum;
	} else {
		return sum/numValues;
	}
}