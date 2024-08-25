#include "fff.h"
#include "timer_s.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_STEP_COUNTER_MAIN_FAKES_LIST(FUNC)      \
    FUNC(readCurrentTick)                 \
    FUNC(flashMessage)                    \
    FUNC(usnprintf)    

typedef __SIZE_TYPE__ size_t;

VALUE_FUNC(unsigned long, readCurrentTick);
VOID_FUNC(flashMessage, char*);
VALUE_FUNC(int, usnprintf, char*, size_t, const char*);