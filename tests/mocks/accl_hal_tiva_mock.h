#include "fff.h"
#include "accl_hal_tiva.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ACC_HAL_TIVA_FAKES_LIST(FUNC)      \
    FUNC(initAcclChip)                   \
    FUNC(getAcclData)                    

VOID_FUNC(initAcclChip);
VOID_FUNC(getAcclData, vector3_t *);

