#ifndef ACCL_HAL_TIVA_H_
#define ACCL_HAL_TIVA_H_

#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;

void initAcclChip(void);

void getAcclData(vector3_t* vector);


#endif