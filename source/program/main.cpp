#include "lib.hpp"
#include "err.hpp"
#include "file_handler.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "hid_handler.hpp"
#include "amx_handler.hpp"
#include "savefile.hpp"
#include "rng_manager.hpp"
#include "patches/learnset.hpp"
#include "patches/permadeath.hpp"
#include "patches/trainer_poke.hpp"
#include "patches/route_restriction.hpp"
#include "patches/field_items.hpp"
#include "patches/wild_pokemon.hpp"
#include "patches/evolution.hpp"
#include "patches/personal_total.hpp"
#include "patches/gifts.hpp"
#ifdef DEBUG
#include "patches/debug.hpp"
#endif

SaveFile save_file;

HOOK_DEFINE_TRAMPOLINE(MainInitHook){
    static void Callback(void* x0, void* x1, void* x2, void* x3) {
        FileHandler::MountSD();
        if (FileHandler::FileExists("sd:/switch/ironmon_save.toml")) {
            std::string save_string;
            FileHandler::ReadFile("sd:/switch/ironmon_save.toml", save_string);
            auto save_table = toml::parse(save_string);
            if (!save_table) {
                EXL_ABORT("Invalid Config");
            }
            save_file.from_table(save_table);
        } else {
            auto rng = RngManager::NewRandomGenerator();
            save_file.rng_seed = rng();
        }
        Orig(x0, x1, x2, x3);
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking environment. */
    exl::hook::Initialize();
    MainInitHook::InstallAtOffset(MainInit_offset);
    install_learnset_patch();
    install_permadeath_patch();
    install_trainer_poke_patch();
    install_route_restriction_patch();
    install_field_items_patch();
    install_wild_pokemon_patch();
    install_evolution_patch();
    install_personal_total_patch();
    install_gifts_patch();
    install_hid_patch();
    install_savefile_patch();
#ifdef DEBUG
    install_debug_patch();
#endif
}

extern "C" NORETURN void exl_exception_entry() {
    /* Note: this is only applicable in the context of applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}