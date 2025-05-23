#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"

HOOK_DEFINE_TRAMPOLINE(RandomLearnsetHook) {
    static void Callback(LearnsetData* learnset_data, u32 species, u32 form) {
        u64 species_form_index = GetSpeciesFormIndex(species, form);
        auto vanilla_items = LearnsetData::WAZAOBOE_TOTAL_LEARNSET_DATA()[species_form_index];
        std::memcpy(learnset_data->learnset_items, vanilla_items, sizeof(learnset_item_t) * 65);
        learnset_data->count = 0;
        for (int i = 0; i < 65; i++) {
            if (learnset_data->learnset_items[i].move_id == -1 && learnset_data->learnset_items[i].level == -1) {
                break;
            }
            // follow the same levels as the vanilla learnset but with random moves
            learnset_data->learnset_items[i].move_id = random_valid_move_id();
            learnset_data->count++;
        }

    }
};

void install_learnset_patch() {
    RandomLearnsetHook::InstallAtOffset(LearnsetData::FetchLearnsetData_offset);
}