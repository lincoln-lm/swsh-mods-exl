#include "lib.hpp"
#include "lib/util/random.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

HOOK_DEFINE_INLINE(PlayShinySound) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active) {
            // ensure shininess is pulled from the OverworldSpec
            OverworldEncount::OverworldSpec* overworld_spec = reinterpret_cast<OverworldEncount::OverworldSpec*>(ctx->X[21] + 0x270);
            if (overworld_spec->shininess == 1) {
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

HOOK_DEFINE_INLINE(SetUpAuraPtcl) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && !global_config.overworld_shiny.shiny_ptcl.empty()) {
            auto ptcl = getFNV1aHashedString(global_config.overworld_shiny.shiny_ptcl.c_str());
            // TODO: symbol something like "AddPtclEffect"
            external<void>(0xcc07f0 - VER_OFF, ctx->X[19], &ptcl, -1);
        }
    }
};

HOOK_DEFINE_INLINE(PlayAuraPtcl) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && !global_config.overworld_shiny.shiny_ptcl.empty()) {
            if (*reinterpret_cast<u8*>(ctx->X[19] + 0x5D8) == 1) {
                ctx->X[20] = getFNV1aHashedString(global_config.overworld_shiny.shiny_ptcl.c_str()).hash;
            }
        }
    }
};

HOOK_DEFINE_INLINE(PlayFishAuraPtcl) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && !global_config.overworld_shiny.shiny_ptcl.empty()) {
            if (*reinterpret_cast<u8*>(ctx->X[19] + 0x530) == 1) {
                ctx->X[20] = getFNV1aHashedString(global_config.overworld_shiny.shiny_ptcl.c_str()).hash;
            }
        }
    }
};

HOOK_DEFINE_INLINE(StopAuraPtcl) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && !global_config.overworld_shiny.shiny_ptcl.empty()) {
            if (*reinterpret_cast<u8*>(ctx->X[19] + 0x5D8) == 1) {
                ctx->X[8] = getFNV1aHashedString(global_config.overworld_shiny.shiny_ptcl.c_str()).hash;
            }
        }
    }
};

HOOK_DEFINE_INLINE(StopFishAuraPtcl) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && !global_config.overworld_shiny.shiny_ptcl.empty()) {
            if (*reinterpret_cast<u8*>(ctx->X[20] + 0x530) == 1) {
                ctx->X[24] = getFNV1aHashedString(global_config.overworld_shiny.shiny_ptcl.c_str()).hash;
            }
        }
    }
};

HOOK_DEFINE_INLINE(RepurposeBrilliantAura) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && !global_config.overworld_shiny.shiny_ptcl.empty()) {
            // check shiny flag or brilliant flag
            ctx->W[8] |= *reinterpret_cast<u8*>(ctx->X[0] + 0x8B0) == 1;
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
            if (!global_config.overworld_shiny.shiny_ptcl.empty()) {
                // check shiny flag or brilliant flag
                ctx->W[8] |= is_shiny;
            }
        }
    }
};

void install_overworld_shiny_patch() {
    PlayShinySound::InstallAtOffset(EncountObject::FromParams_offset+0x194);
    ModifyShinyRate::InstallAtOffset(OverworldEncount::GenerateMainSpec_offset+0x2D8);
    RepurposeBrilliantAura::InstallAtOffset(AuraHandler_offset+0x16C);
    FishAuraAndShinySound::InstallAtOffset(FishAuraHandler_offset+0x238);
    SetUpAuraPtcl::InstallAtOffset(EncountObject_offset+0x4C4);
    SetUpAuraPtcl::InstallAtOffset(FishingPoint_offset+0x114);
    PlayAuraPtcl::InstallAtOffset(UpdatesAura_offset+0x3FC);
    PlayFishAuraPtcl::InstallAtOffset(FishAuraHandler_offset+0x270);
    StopAuraPtcl::InstallAtOffset(UpdatesAura_offset+0x4A8);
    // TODO: label offset for whatever the fishing function does
    StopFishAuraPtcl::InstallAtOffset(0xd6683c-VER_OFF);
}