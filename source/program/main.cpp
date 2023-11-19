#include "lib.hpp"
#include "file_handler.hpp"
#include "external.hpp"
#include "patches/underworld.hpp"
#include "patches/overworld_shiny.hpp"
#include <stdio.h>

HOOK_DEFINE_TRAMPOLINE(MainInitHook){
    static void Callback(void* x0, void* x1, void* x2, void* x3){
        R_ABORT_UNLESS(FileHandler::MountSD());
        // std::string str;
        // R_ABORT_UNLESS(FileHandler::ReadFile("sd:/testing.txt", str));
        Orig(x0, x1, x2, x3);
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    exl::hook::Initialize();
    MainInitHook::InstallAtOffset(MainInit_offset);

    // install_underworld_patch();
    install_overworld_shiny_patch();
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}