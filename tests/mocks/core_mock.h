#include "fff.h"
#include "core.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_CORE_FAKES_LIST(FUNC)         \
    FUNC(readCurrentTick)      \
    FUNC(flashMessage)              \
    FUNC(displayFlashMessage)       \
    FUNC(isFlashing)

VALUE_FUNC(unsigned long, readCurrentTick);
VOID_FUNC(displayFlashMessage);
VALUE_FUNC(bool, isFlashing);
VOID_FUNC(flashMessage, char*);

