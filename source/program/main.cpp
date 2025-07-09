#include "lib.hpp"
#include "err.hpp"
#include "file_handler.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "hid_handler.hpp"
#include "patches/filter_objects.hpp"

HOOK_DEFINE_TRAMPOLINE(MainInitHook){
    static void Callback(void* x0, void* x1, void* x2, void* x3) {
        Orig(x0, x1, x2, x3);
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking environment. */
    exl::hook::Initialize();
    MainInitHook::InstallAtOffset(MainInit_offset);
    install_filter_objects_patch();
}

extern "C" NORETURN void exl_exception_entry() {
    /* Note: this is only applicable in the context of applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}