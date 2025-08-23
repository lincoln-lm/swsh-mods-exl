#pragma once
#include "tomlplusplus/toml.hpp"
#include "util.hpp"
#include <map>

struct PatchConfig {
    bool initialized = false;

    struct {
        std::vector<u32> metronome_moves;
    } metronome;

    bool from_table(toml::parse_result &table) {
        auto moves = table["moves"];
        if (toml::array* arr = moves.as_array()) {
            arr->for_each([this](auto&& el) {
                metronome.metronome_moves.push_back(static_cast<u32>(**el.as_integer()));
            });
        }
        // for (auto&& [k, v] : table)
        // {
        //     u32 base_ability = std::stoi(std::string(k.data()));
        //     std::vector<u32> abilities;
        //     toml::array& arr = *v.as<toml::array>();
        //     for (auto&& el : arr)
        //     {
        //         abilities.push_back(static_cast<u64>(**el.as_integer()));
        //     };
        //     as_one.as_one_abilities.emplace(base_ability,abilities);
        // }
        initialized = true;
        return true;
    }
};

extern PatchConfig global_config;