#include "lib.hpp"
#include "config.hpp"
#include "file_handler.hpp"
#include "external.hpp"
#include "patches/underworld.hpp"
#include "patches/overworld_shiny.hpp"
#include "tomlplusplus/toml.hpp"

PatchConfig global_config;

HOOK_DEFINE_TRAMPOLINE(MainInitHook){
    static void Callback(void* x0, void* x1, void* x2, void* x3){
        R_ABORT_UNLESS(FileHandler::MountSD());
        std::string config_str;
        R_ABORT_UNLESS(FileHandler::ReadFile("sd:/config/swsh-mods-exl/config.toml", config_str));
        auto config = toml::parse(config_str);
        EXL_ASSERT(config);
        global_config.from_table(config);
        Orig(x0, x1, x2, x3);
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    exl::hook::Initialize();
    MainInitHook::InstallAtOffset(MainInit_offset);

    install_underworld_patch();
    install_overworld_shiny_patch();
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}