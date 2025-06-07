#pragma once
#include "tomlplusplus/toml.hpp"
#include <unordered_set>
#include "file_handler.hpp"

struct SaveFile {
    u64 rng_seed;
    bool rng_seed_set;

    s64 settings_menu_state = 0;

    struct {
        bool enabled = true;
        bool match_types = true;
        bool each_level = false;
    } evo_rng;
    struct {
        bool enabled = true;
    } learnset_rng;
    struct {
        bool enabled = true;
    } item_rng;
    struct {
        bool enabled = true;
    } personal_rng;
    struct {
        bool enabled = true;
        bool level_boost = true;
    } trainer_rng;
    struct {
        bool enabled = true;
        bool live = false;
        bool level_boost = true;
    } wild_rng;
    struct {
        bool enabled = true;
    } scripted_rng;
    struct {
        bool enabled = true;
    } gift_rng;
    struct {
        bool enabled = true;
    } model_rng;
    struct {
        bool enabled = true;
    } permadeath;
    struct {
        bool enabled = true;
        std::unordered_set<u64> blacklisted_zones;
    } route_restriction;
    struct {
        bool enabled = true;
    } level_cap_boost;

    #define LOAD_BOOL(name) this->name = table[#name].as_boolean()->get()
    #define LOAD_INT(name) this->name = table[#name].as_integer()->get()
    #define SAVE_BOOL(name) table.insert(#name, name)
    #define SAVE_INT(name) table.insert(#name, name)

    SaveFile() {}
    void from_table(const toml::table& table) {
        s64 seed = table["rng_seed"].as_integer()->get();
        this->rng_seed = static_cast<u64>(seed);
        LOAD_BOOL(rng_seed_set);
        LOAD_INT(settings_menu_state);
        LOAD_BOOL(evo_rng.enabled);
        LOAD_BOOL(evo_rng.match_types);
        LOAD_BOOL(evo_rng.each_level);
        LOAD_BOOL(learnset_rng.enabled);
        LOAD_BOOL(item_rng.enabled);
        LOAD_BOOL(personal_rng.enabled);
        LOAD_BOOL(trainer_rng.enabled);
        LOAD_BOOL(trainer_rng.level_boost);
        LOAD_BOOL(wild_rng.enabled);
        LOAD_BOOL(wild_rng.live);
        LOAD_BOOL(wild_rng.level_boost);
        LOAD_BOOL(scripted_rng.enabled);
        LOAD_BOOL(gift_rng.enabled);
        LOAD_BOOL(model_rng.enabled);
        LOAD_BOOL(permadeath.enabled);
        LOAD_BOOL(route_restriction.enabled);
        if (auto zones = table["route_restriction.blacklisted_zones"].as_array()) {
            for (const auto& zone : *zones) {
                this->route_restriction.blacklisted_zones.insert(static_cast<u64>(zone.as_integer()->get()));
            }
        }
        LOAD_BOOL(level_cap_boost.enabled);
    }
    void to_table(toml::table& table) const {
        toml::array zones;
        for (const auto& zone : route_restriction.blacklisted_zones) {
            zones.push_back(static_cast<s64>(zone));
        }
        table.insert("rng_seed", static_cast<s64>(rng_seed));
        SAVE_BOOL(rng_seed_set);
        SAVE_INT(settings_menu_state);
        SAVE_BOOL(evo_rng.enabled);
        SAVE_BOOL(evo_rng.match_types);
        SAVE_BOOL(evo_rng.each_level);
        SAVE_BOOL(learnset_rng.enabled);
        SAVE_BOOL(item_rng.enabled);
        SAVE_BOOL(personal_rng.enabled);
        SAVE_BOOL(trainer_rng.enabled);
        SAVE_BOOL(trainer_rng.level_boost);
        SAVE_BOOL(wild_rng.enabled);
        SAVE_BOOL(wild_rng.live);
        SAVE_BOOL(wild_rng.level_boost);
        SAVE_BOOL(scripted_rng.enabled);
        SAVE_BOOL(gift_rng.enabled);
        SAVE_BOOL(model_rng.enabled);
        SAVE_BOOL(permadeath.enabled);
        SAVE_BOOL(route_restriction.enabled);
        table.insert("route_restriction.blacklisted_zones", zones);
        SAVE_BOOL(level_cap_boost.enabled);
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