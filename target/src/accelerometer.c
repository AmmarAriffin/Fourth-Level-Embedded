/*
* 
* Accelerometer Module
* 
*
*
*/


#include "accelerometer.h"
#include "circBufT.h"


typedef struct {
    circBuf_t* AccelX;
    circBuf_t* AccelY;
    circBuf_t* AccelZ;
} AccelBuf;


#define VECTOR_SIZE 10

static AccelBuf AccelBuffer;

void initAccel(void)
{
    initCircBuf(&(AccelBuffer.AccelX), VECTOR_SIZE);
    initCircBuf(&(AccelBuffer.AccelY), VECTOR_SIZE);
    initCircBuf(&(AccelBuffer.AccelZ), VECTOR_SIZE);
}
