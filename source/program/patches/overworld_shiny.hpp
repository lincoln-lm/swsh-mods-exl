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
            // in the base game W[9] should only be set for following pokemon
            if (global_config.overworld_shiny.play_sound_for_following || !ctx->W[9]) {
                if (overworld_spec->shininess == 1) {
                    // sounds close enough to shiny sparkles
                    SendCommand(global_config.overworld_shiny.sound.c_str());
                }
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
            if (*reinterpret_cast<u8*>(ctx->X[19] + 0x8B0) == 1) {
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
            if (*reinterpret_cast<u8*>(ctx->X[19] + 0x8B0) == 1) {
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
        if (global_config.overworld_shiny.active) {
            bool show_aura = false;
            if (global_config.overworld_shiny.show_aura_for_brilliants) {
                // default brilliant check
                show_aura |= ctx->W[8];
            }
            if (!global_config.overworld_shiny.shiny_ptcl.empty()) {
                // shinies use aura for sparkles
                // !is_following
                if (global_config.overworld_shiny.show_ptcl_for_following || !*reinterpret_cast<bool*>(ctx->X[19] + 0xd58)) {
                    show_aura |= *reinterpret_cast<u8*>(ctx->X[0] + 0x8B0) == 1;
                }
            }
            // check shiny flag or brilliant flag
            ctx->W[8] = show_aura;
        }
    }
};

HOOK_DEFINE_INLINE(FishAuraAndShinySound) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active) {
            bool show_aura = false;
            bool is_shiny = *reinterpret_cast<u8*>(ctx->X[19] + 0x530) == 1;
            if (is_shiny) {
                SendCommand(global_config.overworld_shiny.sound.c_str());
            }
            if (global_config.overworld_shiny.show_aura_for_brilliants) {
                // default brilliant check
                show_aura |= ctx->W[8];
            }
            if (!global_config.overworld_shiny.shiny_ptcl.empty()) {
                // shinies use aura for sparkles
                show_aura |= is_shiny;
            }
            ctx->W[8] = show_aura;
        }
    }
};

HOOK_DEFINE_INLINE(IncludeBattleSoundBank) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && global_config.overworld_shiny.include_battle_sounds) {
            auto bank = getFNV1aHashedString("Battle_System_Flow.bnk");
            // TODO: unload sound bank later?
            LoadSoundBank(ctx->X[0], &bank, 0, 0, 0);
        }
    }
};

HOOK_DEFINE_INLINE(DisableVanillaBattleSoundBank) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.overworld_shiny.active && global_config.overworld_shiny.include_battle_sounds) {
            // Battle_System_Flow.bnk is already loaded and additionally should not be unloaded at the end of a battle
            // replacing this with a dummy string that doesn't point to a valid file fixes both issues
            auto dummy_bank = getFNV1aHashedString("");
            auto bank = reinterpret_cast<hashed_string_t*>(ctx->X[1]);
            bank->hash = dummy_bank.hash;
            bank->length = dummy_bank.length;
            bank->string = dummy_bank.string;
            bank->unk = dummy_bank.unk;
        }
    }
};

void install_overworld_shiny_patch() {
    DisableVanillaBattleSoundBank::InstallAtOffset(SetUpBattleEnvironment_offset+0x608);
    IncludeBattleSoundBank::InstallAtOffset(InitSoundEngine_offset+0x2B4);
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