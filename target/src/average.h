/*
    Averager



*/

#ifndef AVERAGE_H_
#define AVERAGE_H_

#include "circBufT.h"

typedef struct {
  circBuf_t buffer;
  int32_t sum;
} averager;

void initAverager(averager* averageStruct, uint32_t size);

void storeData(averager* averageStruct, int32_t value);

int32_t getAverage(averager* averageStruct);

#endif /* AVERAGE_H_ */