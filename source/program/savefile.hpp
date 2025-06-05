#pragma once
#include "tomlplusplus/toml.hpp"
#include <unordered_set>
#include "file_handler.hpp"

struct SaveFile {
    u64 rng_seed;
    bool rng_seed_set;

    bool evolution_randomization_enabled = true;

    bool learnset_randomization_enabled = true;

    bool item_randomization_enabled = true;

    bool personal_randomization_enabled = true;

    bool trainer_poke_randomization_enabled = true;

    bool wild_pokemon_randomization_enabled = true;

    bool model_randomization_enabled = true;

    bool permadeath_enabled = true;

    bool route_restriction_enabled = true;
    std::unordered_set<u64> blacklisted_zones;

    bool level_cap_enabled = true;
    
    SaveFile() {}
    void from_table(const toml::table& table) {
        s64 seed = table["rng_seed"].as_integer()->get();
        this->rng_seed = static_cast<u64>(seed);
        this->rng_seed_set = table["rng_seed_set"].as_boolean()->get();
        this->evolution_randomization_enabled = table["evolution_randomization_enabled"].as_boolean()->get();
        this->learnset_randomization_enabled = table["learnset_randomization_enabled"].as_boolean()->get();
        this->item_randomization_enabled = table["item_randomization_enabled"].as_boolean()->get();
        this->personal_randomization_enabled = table["personal_randomization_enabled"].as_boolean()->get();
        this->trainer_poke_randomization_enabled = table["trainer_poke_randomization_enabled"].as_boolean()->get();
        this->wild_pokemon_randomization_enabled = table["wild_pokemon_randomization_enabled"].as_boolean()->get();
        this->model_randomization_enabled = table["model_randomization_enabled"].as_boolean()->get();
        this->permadeath_enabled = table["permadeath_enabled"].as_boolean()->get();
        this->route_restriction_enabled = table["route_restriction_enabled"].as_boolean()->get();
        if (auto zones = table["blacklisted_zones"].as_array()) {
            for (const auto& zone : *zones) {
                this->blacklisted_zones.insert(static_cast<u64>(zone.as_integer()->get()));
            }
        }
        this->level_cap_enabled = table["level_cap_enabled"].as_boolean()->get();
    }
    void to_table(toml::table& table) const {
        toml::array zones;
        for (const auto& zone : blacklisted_zones) {
            zones.push_back(static_cast<s64>(zone));
        }
        table.insert("rng_seed", static_cast<s64>(rng_seed));
        table.insert("rng_seed_set", rng_seed_set);
        table.insert("evolution_randomization_enabled", evolution_randomization_enabled);
        table.insert("learnset_randomization_enabled", learnset_randomization_enabled);
        table.insert("item_randomization_enabled", item_randomization_enabled);
        table.insert("personal_randomization_enabled", personal_randomization_enabled);
        table.insert("trainer_poke_randomization_enabled", trainer_poke_randomization_enabled);
        table.insert("wild_pokemon_randomization_enabled", wild_pokemon_randomization_enabled);
        table.insert("model_randomization_enabled", model_randomization_enabled);
        table.insert("permadeath_enabled", permadeath_enabled);
        table.insert("route_restriction_enabled", route_restriction_enabled);
        table.insert("blacklisted_zones", zones);
        table.insert("level_cap_enabled", level_cap_enabled);
    }
};

extern SaveFile save_file;

HOOK_DEFINE_INLINE(SaveSaveFile) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (strcmp(*reinterpret_cast<char**>(ctx->X[0] + 0x70), "main") == 0) {
            toml::table save_table;
            save_file.to_table(save_table);
            std::stringstream save_string;
            save_string << save_table;
            FileHandler::WriteFile(
                "sd:/switch/ironmon_save.toml",
                save_string.str()
            );
        }
    }
};

void install_savefile_patch() {
    SaveSaveFile::InstallAtOffset(0x5e7e20);
}