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
    u16 model_id;
    u8 padding_0[8];
    u16 hatch_species;
    u16 local_form_index;
    u16 regional_flags;
    u16 pokedex_index;
    u16 regional_form_index;
    u16 form;
    u8 padding_1[70];
    u8 armor_tutor[4];
    u16 armor_dex_index;
    u16 crown_dex_index;
} PACKED;
static_assert(sizeof(personal_info_t) == 0xB0);

HOOK_DEFINE_TRAMPOLINE(FetchPersonalInfoHook) {
    static void Callback(personal_info_t* personal_info, u32 species, u16 form) {
        Orig(personal_info, species, form);
        if (!save_file.personal_rng.enabled) return;

        const std::string seed = std::format("personal_total_{}_{}", species, form);
        auto rng = RngManager::NewRandomGenerator(seed);
        // bst calc taken from UPR ZX
        u64 bst;
        // shedinja gets special handling
        bool is_shedinja = species == 292;
        if (is_shedinja) {
            bst = std::accumulate(
                personal_info->base_stats,
                personal_info->base_stats + 6,
                0
            ) - 51;
        } else {
            bst = std::accumulate(
                personal_info->base_stats,
                personal_info->base_stats + 6,
                0
            ) - 70;
        }
        std::array<u16, 6> base_stats;
        do {
            f64 hp_weight = rng.RandDouble();
            f64 attack_weight = rng.RandDouble();
            f64 defense_weight = rng.RandDouble();
            f64 special_attack_weight = rng.RandDouble();
            f64 special_defense_weight = rng.RandDouble();
            f64 speed_weight = rng.RandDouble();
            f64 total_weight = (
                (is_shedinja ? 0.0d : hp_weight)
                + attack_weight
                + defense_weight
                + special_attack_weight
                + special_defense_weight
                + speed_weight
            );
            if (is_shedinja) {
                base_stats[0] = 1;
            } else {
                base_stats[0] = std::max(1.0, std::round(hp_weight / total_weight * bst)) + 20;
            }
            base_stats[1] = std::max(1.0, std::round(attack_weight / total_weight * bst)) + 10;
            base_stats[2] = std::max(1.0, std::round(defense_weight / total_weight * bst)) + 10;
            base_stats[3] = std::max(1.0, std::round(special_attack_weight / total_weight * bst)) + 10;
            base_stats[4] = std::max(1.0, std::round(special_defense_weight / total_weight * bst)) + 10;
            base_stats[5] = std::max(1.0, std::round(speed_weight / total_weight * bst)) + 10;
        } while (
            std::any_of(
                base_stats.begin(),
                base_stats.end(),
                [](u16 stat) { return stat > 255; }
            )
        );
        std::copy(
            base_stats.begin(),
            base_stats.end(),
            personal_info->base_stats
        );
        personal_info->abilities[0] = rng.RandAbility();
        personal_info->abilities[1] = rng.RandAbility();
        personal_info->abilities[2] = rng.RandAbility();
        personal_info->held_items[0] = rng.RandHeldItem();
        personal_info->held_items[1] = rng.RandHeldItem();
        personal_info->held_items[2] = rng.RandHeldItem();
        // TODO: randomize TR/TM/Tutors?
    }
};

void install_personal_total_patch() {
    FetchPersonalInfoHook::InstallAtOffset(0x77f540);
}