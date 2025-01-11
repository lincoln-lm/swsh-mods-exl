#include "lib.hpp"
#include "err.hpp"
#include "config.hpp"
#include "file_handler.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "hid_handler.hpp"
#include "amx_handler.hpp"
#include "patches/uncap_level.hpp"
#include "patches/camera_tweaks.hpp"
#include "patches/randomizer.hpp"
#include "patches/underworld.hpp"
#include "patches/overworld_shiny.hpp"
#include "patches/freecam.hpp"
#include "patches/glimwood_overworld.hpp"
#include "patches/synchro_mode.hpp"
#include "patches/extended_following.hpp"
#include "patches/fishing_tweaks.hpp"
#ifdef DEBUG
#include "patches/debug.hpp"
#endif
#include "tomlplusplus/toml.hpp"

PatchConfig global_config;

HOOK_DEFINE_TRAMPOLINE(MainInitHook){
    static void Callback(void* x0, void* x1, void* x2, void* x3){
        R_ABORT_UNLESS(FileHandler::MountSD());
        std::string config_str;
        if (R_FAILED(FileHandler::ReadFile("sd:/config/swsh-mods-exl/config.toml", config_str))) {
            nn::err::ApplicationErrorArg err(
                nn::err::MakeErrorCode(nn::err::ErrorCodeCategoryType::unk1, 0x420), "Failed to read config!",
                "Please ensure 'sd:/config/swsh-mods-exl/config.toml' exists",
                nn::settings::LanguageCode::Make(nn::settings::Language::Language_English)
            );
            nn::err::ShowApplicationError(err);
            EXL_ABORT("Failed to read config.");
        }
        auto config = toml::parse(config_str);
        if (!config) {
            nn::err::ApplicationErrorArg err(
                nn::err::MakeErrorCode(nn::err::ErrorCodeCategoryType::unk1, 0x420), "Invalid config!",
                "Please ensure 'sd:/config/swsh-mods-exl/config.toml' contains a valid TOML",
                nn::settings::LanguageCode::Make(nn::settings::Language::Language_English)
            );
            nn::err::ShowApplicationError(err);
            EXL_ABORT("Invalid config.");
        }
        global_config.from_table(config);
        Orig(x0, x1, x2, x3);
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking environment. */
    exl::hook::Initialize();
    MainInitHook::InstallAtOffset(MainInit_offset);
    install_hid_patch();
    install_amx_patch();

    install_underworld_patch();
    install_overworld_shiny_patch();
    install_randomizer_patch();
    install_camera_tweaks_patch();
    install_uncap_level_patch();
    install_freecam_patch();
    install_glimwood_overworld_patch();
    install_synchro_mode_patch();
    install_extended_following_patch();
    install_fishing_tweaks_patch();
    #ifdef DEBUG
    install_debug_patch();
    #endif
}

extern "C" NORETURN void exl_exception_entry() {
    /* Note: this is only applicable in the context of applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}