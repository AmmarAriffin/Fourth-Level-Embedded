/*
* 
* Accelerometer Module
* 
*
*
*/


#include "accelerometer.h"
#include "average.h"
#include "accl_hal_tiva.h"

/*******************************************
 *      Local prototypes (not to be used elsewhere)
 *******************************************/
typedef struct {
    averager AccelX;
    averager AccelY;
    averager AccelZ;
} AccelBuffers;

static AccelBuffers AccelBuffer;

// For temporarily storing values obtained from accelerometer
// in a memory safe way
static vector3_t vector; 

/*******************************************
 *******************************************
 *******************************************/

#define ACCEL_BUFFER_SIZE 25


void initAccelBuffer(void)
{
    initAcclChip();
    initAverager(&(AccelBuffer.AccelX), ACCEL_BUFFER_SIZE);
    initAverager(&(AccelBuffer.AccelY), ACCEL_BUFFER_SIZE);
    initAverager(&(AccelBuffer.AccelZ), ACCEL_BUFFER_SIZE);
}



vector3_t getAverageAccel(void)
{
    vector3_t result;
    result.x = getAverage(&(AccelBuffer.AccelX));
    result.y = getAverage(&(AccelBuffer.AccelY));
    result.z = getAverage(&(AccelBuffer.AccelZ));

    return result;
}


/* Helper Local Functions */
// Not going to use this function outside of pollAccelData
void writeAccelData(void)
{
    storeData(&(AccelBuffer.AccelX), vector.x);
    storeData(&(AccelBuffer.AccelY), vector.y);
    storeData(&(AccelBuffer.AccelZ), vector.z);
}

void pollAccelData(void)
{
    getAcclData(&vector);
    writeAccelData();
}




