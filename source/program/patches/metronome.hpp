#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "util.hpp"
#include "config.hpp"

extern PatchConfig global_config;

HOOK_DEFINE_TRAMPOLINE(ReplaceMetronomeTable) {
    static u32* Callback(u32* size_out) {
        if (!global_config.initialized) Orig(size_out);
        else {
            *size_out = global_config.metronome.metronome_moves.size();
            return global_config.metronome.metronome_moves.data();
        }
    }
};

void install_metronome_patch() {
    ReplaceMetronomeTable::InstallAtOffset(0x7ddbf0);
}