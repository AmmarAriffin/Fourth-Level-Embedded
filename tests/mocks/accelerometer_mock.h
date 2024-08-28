#include "fff.h"
#include "accelerometer.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ACCELEROMETER_FAKES_LIST(FUNC)         \
    FUNC(initAccelBuffer)      \
    FUNC(pollAccelData)              \
    FUNC(getAverageAccel)

VOID_FUNC(initAccelBuffer);
VOID_FUNC(pollAccelData);
VALUE_FUNC(vector3_t, getAverageAccel);