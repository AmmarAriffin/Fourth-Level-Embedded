#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "circBufT.h"


#define VECTOR_SIZE 10

typedef struct AccelBuf AccelBuf;
typedef struct {
    circBuf_t* AccelX;
    circBuf_t* AccelY;
    circBuf_t* AccelZ;
} AccelBuf;




#endif