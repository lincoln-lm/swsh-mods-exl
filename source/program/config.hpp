#pragma once
#include "tomlplusplus/toml.hpp"
#include "util.hpp"
#include <map>

struct PatchConfig {
    bool initialized = false;

    struct {
        std::map<u32, std::vector<u32>> as_one_abilities;
    } as_one;

    bool from_table(toml::parse_result &table) {
        for (auto&& [k, v] : table)
        {
            u32 base_ability = std::stoi(std::string(k.data()));
            std::vector<u32> abilities;
            toml::array& arr = *v.as<toml::array>();
            for (auto&& el : arr)
            {
                abilities.push_back(static_cast<u64>(**el.as_integer()));
            };
            as_one.as_one_abilities.emplace(base_ability,abilities);
        }
        initialized = true;
        return true;
    }
};

extern PatchConfig global_config;