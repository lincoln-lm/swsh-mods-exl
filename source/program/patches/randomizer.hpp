#include "lib.hpp"
#include "lib/util/random.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "util.hpp"
#include "config.hpp"

std::tuple<u32, u16> random_species_and_form() {
    u32 species;
    u16 form;
    do {
        species = (exl::util::GetRandomU64() % 898) + 1;
        PersonalInfo::FetchInfo(species, 0);
        u32 form_count = PersonalInfo::GetField(PersonalInfo::InfoField::FORM_COUNT);
        form = exl::util::GetRandomU64() % form_count;
    } while (!PersonalInfo::isInGame(species, form));
    return {species, form};
}


HOOK_DEFINE_TRAMPOLINE(RandomizeSlotSpawns) {
    static void Callback(long param_1, OverworldEncount::OverworldSpec *overworld_spec, OverworldEncount::encounter_slot_t *encounter_slot, int minimum_level, int maximum_level, long param_6) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.randomizer.active) {
            auto [species, form] = random_species_and_form();
            encounter_slot->species = species;
            encounter_slot->form = form;
        }
        Orig(param_1, overworld_spec, encounter_slot, minimum_level, maximum_level, param_6);
    }
};

HOOK_DEFINE_TRAMPOLINE(RandomizeGimmickSpawns) {
    static void Callback(OverworldEncount::GimmickSpec *gimmick_spec, OverworldEncount::OverworldSpec* overworld_spec) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.randomizer.active) {
            auto [species, form] = random_species_and_form();
            gimmick_spec->species = species;
            gimmick_spec->form = form;
        }
        Orig(gimmick_spec, overworld_spec);
    }
};

void install_randomizer_patch() {
    RandomizeSlotSpawns::InstallAtOffset(OverworldEncount::GenerateBasicSpec_offset);
    RandomizeGimmickSpawns::InstallAtOffset(OverworldEncount::InitGimmickSpec_offset);
}