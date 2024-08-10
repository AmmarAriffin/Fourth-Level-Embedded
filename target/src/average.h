/*
    Averager



*/

#ifndef AVERAGE_H_
#define AVERAGE_H_

#include "circBufT.h"

typedef struct {
  circBuf_t buffer;
} averager;

void initAverager(averager* averageStruct);

void storeData(averager* averageStruct, uint32_t value);

uint32_t getAverage(averager* averageStruct);

#endif /* AVERAGE_H_ */