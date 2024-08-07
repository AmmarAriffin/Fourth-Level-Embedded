#include "fff.h"
#include "adc_hal_tiva.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ADC_HAL_TIVA_FAKES_LIST(FUNC)      \
    FUNC(adcHalRegister)                   \
    FUNC(adcIntCallback)                   \
    FUNC(adcHalStartConversion)               

VOID_FUNC(adcHalRegister, uint32_t, callback);
VOID_FUNC(adcIntCallback);
VOID_FUNC(adcHalStartConversion, uint32_t);
