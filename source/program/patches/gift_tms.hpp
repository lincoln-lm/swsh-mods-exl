#include "amx_handler.hpp"
#include "lib.hpp"
#include "external.hpp"
#include "savefile.hpp"
#include "rng_manager.hpp"
#include "symbols.hpp"
#include "util.hpp"

struct TM_SCRIPT_IDENTIFIER {
    std::string identifier;
    u32 cip;
    u64 old;
    u32 arg;
};

// TODO: verify source amx? probably dont need to
// TODO: rest of tms
static std::vector<TM_SCRIPT_IDENTIFIER> TM_IDENTIFIERS = {
    { "hop_swift", 0x9258, 367, 0 },
    { "hop_swift", 0x92B8, 367, 0 },
    { "hop_swift", 0x21d8, 367, 2 },
    { "hop_swift", 0x8ff8, 367, 0 },
    { "hop_swift", 0x2228, 367, 2 },
    { "hop_swift", 0x94d0, 367, 0 },
    { "hop_swift", 0x8af8, 367, 0 },
    { "hop_swift", 0x8b90, 367, 0 },
    { "hop_swift", 0xbd80, 367, 0 },
};

void replace_tm(AMX::AMX_INSTANCE* instance, u64* args) {
    if (!save_file.gift_tms_rng.enabled) return;
    u64 arg_count = args[0] / 8;
    for (auto& identifier : TM_IDENTIFIERS) {
        if (instance->cip != identifier.cip)
            continue;
        if (identifier.arg >= arg_count)
            continue;
        if (args[identifier.arg + 1] != identifier.old)
            continue;
        auto rng = RngManager::NewRandomGenerator(identifier.identifier);
        args[identifier.arg + 1] = rng.RandTM();
    }
}

void install_gift_tms_patch() {
    AMX::add_amx_callback(&replace_tm);
}
