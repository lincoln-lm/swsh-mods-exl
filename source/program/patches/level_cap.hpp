#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"

HOOK_DEFINE_INLINE(TweakLevelCap) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.level_cap_enabled) return;
        ctx->W[8] *= 1.5;
    }
};

void install_level_cap_patch() {
    TweakLevelCap::InstallAtOffset(GetLevelCap_0_offset + 0x20);
    TweakLevelCap::InstallAtOffset(GetLevelCap_1_offset + 0x64);
}