#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

HOOK_DEFINE_INLINE(RemoveLevelCap) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.uncap_level.active) {
            // set level cap check to 100
            ctx->W[0] = 100;
        }
    }
};

HOOK_DEFINE_INLINE(FullyRemoveLevelCap) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.uncap_level.active && global_config.uncap_level.fully) {
            // set level cap check to 100
            ctx->W[0] = 100;
        }
    }
};

void install_uncap_level_patch() {
    RemoveLevelCap::InstallAtOffset(OverworldEncount::GenerateBasicSpec_offset + 0xd4);
    RemoveLevelCap::InstallAtOffset(OverworldEncount::InitGimmickSpec_offset + 0x98);
    RemoveLevelCap::InstallAtOffset(OverworldEncount::GenerateMainSpec_offset + 0x26c);
    FullyRemoveLevelCap::InstallAtOffset(GetLevelCap_0_offset + 0x28);
    FullyRemoveLevelCap::InstallAtOffset(GetLevelCap_1_offset + 0x6C);
}