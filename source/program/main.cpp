#include "lib.hpp"
#include "err.hpp"
#include "file_handler.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "hid_handler.hpp"
#include "amx_handler.hpp"
#include "patches/learnset.hpp"
#ifdef DEBUG
#include "patches/debug.hpp"
#endif

HOOK_DEFINE_TRAMPOLINE(MainInitHook){
    static void Callback(void* x0, void* x1, void* x2, void* x3){
        Orig(x0, x1, x2, x3);
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking environment. */
    exl::hook::Initialize();
    MainInitHook::InstallAtOffset(MainInit_offset);
    install_learnset_patch();
#ifdef DEBUG
    install_debug_patch();
#endif
}

extern "C" NORETURN void exl_exception_entry() {
    /* Note: this is only applicable in the context of applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}