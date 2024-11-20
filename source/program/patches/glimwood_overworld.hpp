#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"
#include <unordered_set>

const std::unordered_set<u64> glimwood_spawners = {
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_0").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_1").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_2").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_3").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_4").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_5").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_6").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_7").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_8").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_9").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_10").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_11").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_12").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_13").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_14").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_15").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_16").hash,
    getFNV1aHashedString("z_d0401_SymbolEncountPokemonSpawner_D04_Grass_01_17").hash,
};

HOOK_DEFINE_INLINE(PatchMaximumSpawns) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.glimwood_overworld.active) {
            auto spawner = reinterpret_cast<Field::EncountSpawner*>(ctx->X[19]);
            if (glimwood_spawners.find(spawner->unique_hash) != glimwood_spawners.end()) {
                spawner->maximum_symbol_encounts = global_config.glimwood_overworld.maximum_spawns;
            }
        }
    }
};

void install_glimwood_overworld_patch() {
    PatchMaximumSpawns::InstallAtOffset(Field::EncountSpawner::EncountSpawner_offset + 0x150);
}