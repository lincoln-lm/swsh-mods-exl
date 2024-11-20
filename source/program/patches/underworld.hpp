#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

// TODO: this feels hacky
s32 current_spawner_type = -1;

HOOK_DEFINE_TRAMPOLINE(SwapSymbolTableHash) {
    static void Callback(void* param_1, OverworldEncount::encounter_tables_t* encounter_tables, u64 unused, void* overworld_spec) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.underworld.active) {
            // SKY and WATER
            if (current_spawner_type != 1 && current_spawner_type != 2) {
                // pull the hidden table based on the player's current location
                u64 hidden_encount_table_hash = Field::FetchZoneHash();
                *encounter_tables = OverworldEncount::FetchSymbolEncountTable(0, &hidden_encount_table_hash);
            }
            current_spawner_type = -1;
        }

        Orig(param_1, encounter_tables, unused, overworld_spec);
    }
};

HOOK_DEFINE_INLINE(DisableEmptyTableCheck) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.underworld.active) {
            // patch first check to always treat the encounter table as valid
            // this allows spawners with empty/invalid tables to make it to the SwapSymbolTableHash trampoline
            // & is harmless as GenerateSymbolEncountParam re-checks validity
            ctx->W[8] = true;
        }
    }
};

HOOK_DEFINE_INLINE(SwapSymbolTableAccess) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.underworld.active) {
            // SKY and WATER
            if (current_spawner_type != 1 && current_spawner_type != 2) {
                // replace offset that points to the symbol hashmap (+0x2a8) to the hidden hashmap (+0x198)
                // causes all spawners to be initialized with empty tables as their symbol hash won't be in the hidden map
                // this is fine as the table is refreshed every spawn
                ctx->X[8] = *reinterpret_cast<long*>(ctx->X[20] + 0x198);
            }
            current_spawner_type = -1;
        }
    }
};

HOOK_DEFINE_INLINE(VerifySpawnerType) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.underworld.active) {
            current_spawner_type = *reinterpret_cast<s32*>(ctx->X[0] + 0x3b0);
        }
    }
};

HOOK_DEFINE_INLINE(VerifySpawnerTypeInit) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.underworld.active) {
            current_spawner_type = *reinterpret_cast<s32*>(ctx->X[19] + 0x3b0);
        }
    }
};

void install_underworld_patch() {
    VerifySpawnerTypeInit::InstallAtOffset(EncountSpawnerInit_offset + 0x13c);
    VerifySpawnerType::InstallAtOffset(OverworldEncount::TryGenerateSymbolEncount_offset);
    SwapSymbolTableHash::InstallAtOffset(OverworldEncount::GenerateSymbolEncountParam_offset);
    SwapSymbolTableAccess::InstallAtOffset(OverworldEncount::FetchSymbolEncountTable_offset + 0x54);
    DisableEmptyTableCheck::InstallAtOffset(OverworldEncount::TryGenerateSymbolEncount_offset + 0x1C);
}