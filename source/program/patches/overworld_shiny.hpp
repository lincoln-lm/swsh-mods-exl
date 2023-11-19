#include "lib.hpp"
#include "lib/util/random.hpp"
#include "external.hpp"
#include "symbols.hpp"

// auto shiny_sound = "Play_Ba_sys_rare_effect";

// auto shiny_sound = "Play_Audience_cheer_riser_rnd";
// auto shiny_sound = "Play_UI_netbattle_rank_up";
// auto shiny_sound = "Play_UI_Lcircuit_Goal";
// auto shiny_sound = "Play_Camp_Cooking_Explosion";

HOOK_DEFINE_INLINE(PlayShinySound) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (ctx->W[25] == 1) {
            // sounds close enough to shiny sparkles
            SendCommand("Play_Camp_Cooking_Explosion");
        }
        // always show shininess
        ctx->W[9] = true;
    }
};

HOOK_DEFINE_INLINE(ModifyShinyRate) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        // 15%
        ctx->W[24] = (exl::util::GetRandomU64() % 100) < 15;
    }
};

HOOK_DEFINE_INLINE(RepurposeBrilliantAura) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        // check shiny flag rather than brilliant
        ctx->W[8] = *reinterpret_cast<u8*>(ctx->X[0] + 0x8B0) == 1;
    }
};

void install_overworld_shiny_patch() {
    PlayShinySound::InstallAtOffset(EncountObject::FromParams_offset+0x194);
    ModifyShinyRate::InstallAtOffset(OverworldEncount::GenerateMainSpec_offset+0x2D8);
    RepurposeBrilliantAura::InstallAtOffset(AuraHandler_offset+0x16C);
}