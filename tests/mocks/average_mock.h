#include "fff.h"
#include "average.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_AVERAGE_FAKES_LIST(FUNC)         \
    FUNC(getAverage)      \
    FUNC(initAverager)              \
    FUNC(storeData)

VALUE_FUNC(int32_t, getAverage, averager*);
VOID_FUNC(initAverager, averager*);
VOID_FUNC(storeData, averager*, int32_t);