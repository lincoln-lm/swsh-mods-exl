#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"

s16 random_valid_move_id() {
    // TODO: deterministic rng based on species/form & seed
    // TODO: ensure unique moves?
    MoveIdHolder move_id_holder = {
        .unk = 0,
        .move_id = 0,
    };
    do {
        move_id_holder.move_id = (exl::util::GetRandomU64() % 820) + 1;
    } while (!move_id_holder.IsMoveUsable());
    return move_id_holder.move_id;
}

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