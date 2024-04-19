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

    struct {
        bool active;
        float adjustment_speed;
        float min_pitch;
        float max_pitch;
        float min_distance;
        float max_distance;
    } camera_tweaks;

    struct {
        bool active;
        bool fully;
    } uncap_level;

    struct {
        bool active;
    } freecam;

    void from_table(toml::parse_result &table) {
        underworld.active = table["underworld"]["active"].value_or(false);

        overworld_shiny.active = table["overworld_shiny"]["active"].value_or(false);
        overworld_shiny.boosted_percentage = table["overworld_shiny"]["boosted_percentage"].value_or(0);
        overworld_shiny.repurpose_aura = table["overworld_shiny"]["repurpose_aura"].value_or(true);
        overworld_shiny.sound = table["overworld_shiny"]["sound"].value_or("Play_Camp_Cooking_Explosion");

        randomizer.active = table["randomizer"]["active"].value_or(false);

        camera_tweaks.active = table["camera_tweaks"]["active"].value_or(false);
        camera_tweaks.adjustment_speed = table["camera_tweaks"]["adjustment_speed"].value_or(3.0);
        camera_tweaks.min_pitch = table["camera_tweaks"]["min_pitch"].value_or(-20.0);
        camera_tweaks.max_pitch = table["camera_tweaks"]["max_pitch"].value_or(-5.0);
        camera_tweaks.min_distance = table["camera_tweaks"]["min_distance"].value_or(370.0);
        camera_tweaks.max_distance = table["camera_tweaks"]["max_distance"].value_or(800.0);

        uncap_level.active = table["uncap_level"]["active"].value_or(false);
        uncap_level.fully = table["uncap_level"]["fully"].value_or(true);

        freecam.active = table["freecam"]["active"].value_or(false);

        initialized = true;
    }
};

extern PatchConfig global_config;