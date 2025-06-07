#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

static bool animation_requested = false;
static int current_animation_index = 0;

HOOK_DEFINE_INLINE(LogAnimationRequested) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (!global_config.dex_animations.active);
        animation_requested = true;
        if (ctx->W[20] != 0) {
            current_animation_index++;
        }
    }
};

struct animation_entry_t {
    u64 unk_0;
    hashed_string_t animation_string;
    u64 unk_1;
} PACKED;
static_assert(sizeof(animation_entry_t) == 0x30);

HOOK_DEFINE_INLINE(SwapAnimation) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (!global_config.dex_animations.active);
        if (!animation_requested) return;
        animation_requested = false;
        auto animation_entries_start = *reinterpret_cast<animation_entry_t**>(ctx->X[0]+0xb0);
        auto animation_entries_end = *reinterpret_cast<animation_entry_t**>(ctx->X[0]+0xb8);
        auto animation_entries_size = animation_entries_end - animation_entries_start;
        auto valid_animations = std::span(animation_entries_start, animation_entries_size);
        current_animation_index %= animation_entries_size;
        auto target_animation = reinterpret_cast<hashed_string_t*>(ctx->X[1]);
        target_animation->hash = valid_animations[current_animation_index].animation_string.hash;
    }
};


void install_dex_animations_patch() {
    LogAnimationRequested::InstallAtOffset(0x162e1dc - VER_OFF);
    SwapAnimation::InstallAtOffset(0x5b1f30);
}