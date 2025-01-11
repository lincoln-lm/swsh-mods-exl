#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

HOOK_DEFINE_INLINE(KeepFishingChain) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.fishing_tweaks.active && global_config.fishing_tweaks.keep_chain_after_brilliant) {
            // if brilliant index (w8) != 0: clear fishing chain
            ctx->W[8] = 0;
        }
    }
};
void install_fishing_tweaks_patch() {
    // TODO: symbol
    KeepFishingChain::InstallAtOffset(0xd66a94 - VER_OFF);
}
