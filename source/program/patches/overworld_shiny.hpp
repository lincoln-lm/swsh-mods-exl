#include "lib.hpp"
#include "lib/util/random.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

HOOK_DEFINE_INLINE(PlayShinySound) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active) {
            if (ctx->W[25] == 1) {
                // sounds close enough to shiny sparkles
                SendCommand(global_config.overworld_shiny.sound.c_str());
            }
            // always show shininess
            ctx->W[9] = true;
        }
    }
};

HOOK_DEFINE_INLINE(ModifyShinyRate) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && global_config.overworld_shiny.boosted_percentage) {
            ctx->W[24] = (exl::util::GetRandomU64() % 100) < global_config.overworld_shiny.boosted_percentage;
        }
    }
};

HOOK_DEFINE_INLINE(RepurposeBrilliantAura) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && global_config.overworld_shiny.repurpose_aura) {
            // check shiny flag rather than brilliant
            ctx->W[8] = *reinterpret_cast<u8*>(ctx->X[0] + 0x8B0) == 1;
        }
    }
};

HOOK_DEFINE_INLINE(FishAuraAndShinySound) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active) {
            bool is_shiny = *reinterpret_cast<u8*>(ctx->X[19] + 0x530) == 1;
            if (is_shiny) {
                SendCommand(global_config.overworld_shiny.sound.c_str());
            }
            if (global_config.overworld_shiny.repurpose_aura) {
                // check shiny flag rather than brilliant
                ctx->W[8] = is_shiny;
            }
        }
    }
};

void install_overworld_shiny_patch() {
    PlayShinySound::InstallAtOffset(EncountObject::FromParams_offset+0x194);
    ModifyShinyRate::InstallAtOffset(OverworldEncount::GenerateMainSpec_offset+0x2D8);
    RepurposeBrilliantAura::InstallAtOffset(AuraHandler_offset+0x16C);
    FishAuraAndShinySound::InstallAtOffset(FishAuraHandler_offset+0x238);
}