#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "rng_manager.hpp"

HOOK_DEFINE_INLINE(RandomizeGimmickSpec) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.wild_pokemon_randomization_enabled) return;
        // TODO: GimmickSpawner struct
        Field::FieldObject* gimmick_spawner = reinterpret_cast<Field::FieldObject*>(ctx->X[19]);
        // TODO: Field::GetActiveWeather
        s32 weather = external<s32>(0xd05a30 - VER_OFF);
        OverworldEncount::GimmickSpec* gimmick_spec = reinterpret_cast<OverworldEncount::GimmickSpec*>(ctx->X[1]);
        u64 gimmick_spawner_hash = gimmick_spawner->unique_hash;
        const std::string seed = std::format(
            "gimmick_spawner_{}_{}",
            gimmick_spawner_hash,
            weather
        );
        auto rng = RngManager::NewRandomGenerator(seed);
        auto [species, form] = rng.RandSpeciesAndForm();
        gimmick_spec->species = species;
        gimmick_spec->form = form;
        gimmick_spec->level *= 1.5;
        gimmick_spec->ability = rng.RandMax(3);
        gimmick_spec->item = rng.RandHeldItem();
        std::array<s16, 4> moves;
        rng.RandMoves(std::span(moves));
        std::copy(
            moves.begin(),
            moves.end(),
            gimmick_spec->moves
        );
    }
};

HOOK_DEFINE_INLINE(RandomizeHiddenEncounterSlots) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.wild_pokemon_randomization_enabled) return;
        u64 data = ctx->X[19];
        u64 area_hash = *reinterpret_cast<u64*>(data + 0x8);
        OverworldEncount::encounter_tables_t* encounter_tables = reinterpret_cast<OverworldEncount::encounter_tables_t*>(data + 0x10);
        const std::string seed = std::format("hidden_encounter_table_{}", area_hash);
        auto rng = RngManager::NewRandomGenerator(seed);
        for (int table = 0; table < 11; table++) {
            encounter_tables[table].minimum_level *= 1.5;
            encounter_tables[table].maximum_level *= 1.5;
            for (int i = 0; i < 10; i++) {
                auto slot = &(encounter_tables[table].encounter_slots[i]);
                // if (slot->rate == 0) continue;
                slot->rate = 10;
                auto [species, form] = rng.RandSpeciesAndForm();
                slot->species = species;
                slot->form = form;
            }
        }
    }
};

HOOK_DEFINE_INLINE(RandomizeSymbolEncounterSlots) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.wild_pokemon_randomization_enabled) return;
        u64 data = ctx->X[19];
        u64 area_hash = *reinterpret_cast<u64*>(data + 0x8);
        OverworldEncount::encounter_tables_t* encounter_tables = reinterpret_cast<OverworldEncount::encounter_tables_t*>(data + 0x10);
        const std::string seed = std::format("symbol_encounter_table_{}", area_hash);
        auto rng = RngManager::NewRandomGenerator(seed);
        for (int weather = 0; weather < 9; weather++) {
            encounter_tables[weather].minimum_level *= 1.5;
            encounter_tables[weather].maximum_level *= 1.5;
            for (int i = 0; i < 10; i++) {
                auto slot = &(encounter_tables[weather].encounter_slots[i]);
                // if (slot->rate == 0) continue;
                slot->rate = 10;
                auto [species, form] = rng.RandSpeciesAndForm();
                slot->species = species;
                slot->form = form;
            }
        }
    }
};

void install_wild_pokemon_patch() {
    // TODO: symbol
    // on OverworldEncount::GenerateGimmick call
    RandomizeGimmickSpec::InstallAtOffset(0xd5c878 - VER_OFF);
    // TODO: symbol
    // end of FetchHiddenEncountTable
    RandomizeHiddenEncounterSlots::InstallAtOffset(0xd05088 - VER_OFF);
    // TODO: symbol
    // end of FetchSymbolEncountTable
    RandomizeSymbolEncounterSlots::InstallAtOffset(0xd05868 - VER_OFF);
}