#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "rng_manager.hpp"

HOOK_DEFINE_INLINE(RandomizeSparkleItem) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        // TODO: FieldSparkleItem struct
        u64 sparkle_item = ctx->X[26];
        u64 sparkle_item_hash = ctx->X[1];
        s32* item_cutoffs = reinterpret_cast<s32*>(sparkle_item + 0x500);
        u8 item_value = *reinterpret_cast<u8*>(ctx->X[19]);
        u8 item_idx = 0;
        for (u8 i = 0; i < 8; i++) {
            if (i < 7 && item_cutoffs[i] < item_value) continue;
            item_idx = i;
        }
        const std::string seed = std::format("sparkle_item_{}_{}", sparkle_item_hash, item_idx);
        auto rng = RngManager::NewRandomGenerator(std::span(seed));
        // TODO: handling for key items & tms
        ctx->X[2] = rng.RandHeldItem();
        // ctx->X[3] = item_count;
    }
};

HOOK_DEFINE_INLINE(RandomizeBallItem) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        u64 ball_item_hash = ctx->X[1];
        const std::string seed = std::format("ball_item_{}_{}", ball_item_hash, 0);
        auto rng = RngManager::NewRandomGenerator(std::span(seed));
        // TODO: handling for key items & tms
        ctx->X[2] = rng.RandHeldItem();
        // ctx->X[3] = item_count;
    }
};

void install_field_items_patch() {
    // call to pawn script to give item in the sparkle item section of FieldHandleAPress
    RandomizeSparkleItem::InstallAtOffset(0xdc66b0 - VER_OFF);
    // call to pawn script to give item in the ball item section of FieldHandleAPress
    RandomizeBallItem::InstallAtOffset(0xdc64e4 - VER_OFF);
}