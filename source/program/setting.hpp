#pragma once

#include "common.hpp"

#ifdef VERSION_SHIELD
#define EXL_MODULE_NAME "exlaunch_shield"
#else
#define EXL_MODULE_NAME "exlaunch_sword"
#endif

#define EXL_DEBUG
#define EXL_USE_FAKEHEAP

/*
#define EXL_SUPPORTS_REBOOTPAYLOAD
*/

namespace exl::setting {
    /* How large the fake .bss heap will be. */
    constexpr size_t HeapSize = 0x50000;

    /* How large the JIT area will be for hooks. */
    constexpr size_t JitSize = 0x4000;

    /* How large the area will be inline hook pool. */
    constexpr size_t InlinePoolSize = 0x4000;

    /* How large the formatting buffer should be for logging. The buffer will be on the stack. */
    constexpr size_t LogBufferSize = 512;

    /* Sanity checks. */
    static_assert(ALIGN_UP(JitSize, PAGE_SIZE) == JitSize, "");
    static_assert(ALIGN_UP(InlinePoolSize, PAGE_SIZE) == InlinePoolSize, "");
}
