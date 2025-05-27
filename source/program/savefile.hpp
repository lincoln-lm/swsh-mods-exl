#pragma once
#include "tomlplusplus/toml.hpp"
#include <unordered_set>
#include "file_handler.hpp"

struct SaveFile {
    u64 rng_seed;
    std::unordered_set<u64> blacklisted_zones;
    SaveFile() {}
    void from_table(const toml::table& table) {
        s64 seed = table["rng_seed"].as_integer()->get();
        this->rng_seed = static_cast<u64>(seed);
        if (auto zones = table["blacklisted_zones"].as_array()) {
            for (const auto& zone : *zones) {
                this->blacklisted_zones.insert(static_cast<u64>(zone.as_integer()->get()));
            }
        }
    }
    void to_table(toml::table& table) const {
        toml::array zones;
        for (const auto& zone : blacklisted_zones) {
            zones.push_back(static_cast<s64>(zone));
        }
        table.insert("rng_seed", static_cast<s64>(rng_seed));
        table.insert("blacklisted_zones", zones);
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