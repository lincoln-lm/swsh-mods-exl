#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"

struct personal_info_t {
    u8 base_stats[6];
    u8 types[2];
    u8 catch_rate;
    u8 evolution_stage;
    u16 ev_yield;
    u16 held_items[3];
    u8 gender_ratio;
    u8 hatch_cycles;
    u8 base_friendship;
    u8 growth_rate;
    u8 egg_groups[2];
    u16 abilities[3];
    u16 form_stats_index;
    u8 form_count;
    u8 misc;
    u16 base_exp;
    u16 height;
    u16 weight;
    u8 tm[16];
    u8 type_tutors[4];
    u8 tr[16];
    u16 species;
    u8 padding_0[8];
    u16 hatch_species;
    u16 local_form_index;
    u16 regional_flags;
    u16 pokedex_index;
    u16 regional_form_index;
    u8 padding_1[72];
    u8 armor_tutor[4];
    u16 armor_dex_index;
    u16 crown_dex_index;
} PACKED;
static_assert(sizeof(personal_info_t) == 0xB0);

HOOK_DEFINE_INLINE(PersonalTotalHook) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        personal_info_t* personal_total_buffer = reinterpret_cast<personal_info_t*>(ctx->X[1]);
        size_t buffer_size = ctx->X[2];
        std::span<personal_info_t> personal_total_span(
            personal_total_buffer,
            buffer_size / sizeof(personal_info_t)
        );
        for (const auto& [index, personal_info] : personal_total_span | std::views::enumerate) {
            const std::string seed = std::format("personal_total_{}", index);
            auto rng = RngManager::NewRandomGenerator(seed);
            // TODO: randomize abilities
            personal_info.held_items[0] = rng.RandHeldItem();
            personal_info.held_items[1] = rng.RandHeldItem();
            personal_info.held_items[2] = rng.RandHeldItem();
            // TODO: randomize TR/TM/Tutors?
        }
    }
};

void install_personal_total_patch() {
    PersonalTotalHook::InstallAtOffset(0x77f350);
}