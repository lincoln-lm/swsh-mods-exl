#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "util.hpp"
#include "config.hpp"

extern PatchConfig global_config;

HOOK_DEFINE_TRAMPOLINE(RegisterAbilityCallbacks) {
    static void Callback(u64 param_1, u64 param_2, u64 battle_pokemon) {
        if (!global_config.initialized) Orig(param_1, param_2, battle_pokemon);
        u32 fusion_abilities[2];

        u32 battle_pokemon_index = external<u8>(0x7e0a00, battle_pokemon);
        u32 primary_ability = external<u32>(0x7e12a0, battle_pokemon, 0x12);
        auto abilities_it = global_config.as_one.as_one_abilities.find(primary_ability);
        if (abilities_it != global_config.as_one.as_one_abilities.end()) {
            auto abilities = abilities_it->second;
            external<void>(0x83a6a0, param_1, battle_pokemon_index, 266);
            u8 fusion_ability_count = external<u32>(0x8260c0, param_2, battle_pokemon_index, fusion_abilities);
            (void)fusion_ability_count;
            for (auto& ability : abilities) {
                external<void>(0x83a6a0, param_1, battle_pokemon_index, ability);
            }
        } else {
            external<void>(0x83a6a0, param_1, battle_pokemon_index, primary_ability);
            u8 fusion_ability_count = external<u32>(0x8260c0, param_2, battle_pokemon_index, fusion_abilities);
            for (u8 fusion_ability_index = 0; fusion_ability_index < fusion_ability_count; fusion_ability_index++) {
                external<void>(0x83a6a0, param_1, battle_pokemon_index, fusion_abilities[fusion_ability_index]);
            }
        }
    }
};

void install_as_one_patch() {
    RegisterAbilityCallbacks::InstallAtOffset(0x82fa00);
}