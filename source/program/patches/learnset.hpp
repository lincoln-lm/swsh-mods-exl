#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "rng_manager.hpp"

HOOK_DEFINE_TRAMPOLINE(RandomLearnsetHook) {
    static void Callback(LearnsetData* learnset_data, u32 species, u32 form) {
        if (!save_file.learnset_rng.enabled) {
            Orig(learnset_data, species, form);
            return;
        }
        const std::string seed = std::format("learnset_{}_{}", species, form);
        auto rng = RngManager::NewRandomGenerator(seed);
        u64 species_form_index = GetSpeciesFormIndex(species, form);
        auto vanilla_items = LearnsetData::WAZAOBOE_TOTAL_LEARNSET_DATA()[species_form_index];
        std::memcpy(learnset_data->learnset_items, vanilla_items, sizeof(learnset_item_t) * 65);
        learnset_data->count = 0;
        int level_1_move_count = 0;
        for (int i = 0; i < 65; i++) {
            if (learnset_data->learnset_items[i].move_id == -1 && learnset_data->learnset_items[i].level == -1) {
                break;
            }
            if (learnset_data->learnset_items[i].level == 1) {
                level_1_move_count++;
            }
            // replace existing moves
            learnset_data->learnset_items[i].move_id = rng.RandValidMoveId();
            learnset_data->count++;
        }
        if (learnset_data->count) {
            // ensure at least 4 moves at level 1 (if possible)
            if (learnset_data->count < 65 - 3 && level_1_move_count < 4) {
                for (; level_1_move_count < 4; level_1_move_count++) {
                    learnset_data->learnset_items[learnset_data->count].move_id = rng.RandValidMoveId();
                    learnset_data->learnset_items[learnset_data->count].level = 1;
                    learnset_data->count++;
                }
            }
        }
    }
};

void install_learnset_patch() {
    RandomLearnsetHook::InstallAtOffset(LearnsetData::FetchLearnsetData_offset);
}