#pragma once
#include "tomlplusplus/toml.hpp"

struct PatchConfig {
    bool initialized = false;

    struct {
        bool active;
    } underworld;

    struct {
        bool active;
        u32 boosted_percentage;
        bool repurpose_aura;
        std::string sound;
    } overworld_shiny;

    struct {
        bool active;
    } randomizer;

    void from_table(toml::parse_result &table) {
        underworld.active = table["underworld"]["active"].value_or(false);

        overworld_shiny.active = table["overworld_shiny"]["active"].value_or(false);
        overworld_shiny.boosted_percentage = table["overworld_shiny"]["boosted_percentage"].value_or(0);
        overworld_shiny.repurpose_aura = table["overworld_shiny"]["repurpose_aura"].value_or(true);
        overworld_shiny.sound = table["overworld_shiny"]["sound"].value_or("Play_Camp_Cooking_Explosion");

        randomizer.active = table["randomizer"]["active"].value_or(false);

        initialized = true;
    }
};

extern PatchConfig global_config;