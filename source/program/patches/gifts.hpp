#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "rng_manager.hpp"
#include "schemas/encounter_gift_generated.h"

HOOK_DEFINE_TRAMPOLINE(RandomizeGiftPokemon) {
    static inline std::vector<u8> template_data = {};

    static const EncounterGift* Callback(void* table, u64* gift_hash) {
        const EncounterGift* original = Orig(table, gift_hash);
        if (!save_file.gift_rng.enabled) return original;
        auto rng = RngManager::NewRandomGenerator(*gift_hash);
        auto [species, form] = rng.RandSpeciesAndForm();
        auto ball = rng.RandBall();
        auto item = rng.RandHeldItem();
        bool shiny_lock = false;
        auto ability = rng.RandRange(0, 3);
        auto gender = 0; // random
        auto nature = 25; // random
        auto special_move = rng.RandValidMoveId();
        flatbuffers::FlatBufferBuilder builder;
        builder.Finish(
            CreateEncounterGift(
                builder,
                original->is_egg(),
                form,
                original->dynamax_level(),
                ball,
                original->unk_0(),
                original->hash(),
                original->can_gigantamax(),
                item,
                original->level(),
                species,
                original->unk_1(),
                original->memory_code(),
                original->memory_data(),
                original->memory_feel(),
                original->memory_level(),
                original->ot_name_id(),
                original->ot_gender(),
                shiny_lock,
                nature,
                gender,
                original->iv_spe(),
                original->iv_atk(),
                original->iv_def(),
                original->iv_hp(),
                original->iv_spa(),
                original->iv_spd(),
                ability,
                special_move
            )
        );
        u8* data_ptr = builder.GetBufferPointer();
        size_t data_size = builder.GetSize();
        template_data.clear();
        template_data.insert(template_data.end(), data_ptr, data_ptr + data_size);
        return flatbuffers::GetRoot<EncounterGift>(template_data.data());
    }
};


void install_gifts_patch() {
    // TODO: symbol
    // function that fetches the pointer to the gift struct from a lookup table
    RandomizeGiftPokemon::InstallAtOffset(0xe4a370 - VER_OFF);
}