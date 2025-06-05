#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "placement_zone_data.hpp"

// zone hash updated every time an encounter happens
static u64 encounter_zone_hash = 0;

enum WildBattleResult {
    Loss,
    Win,
    Unk2,
    Flee,
    Unk4,
    Capture,
    Unk6,
};

HOOK_DEFINE_INLINE(OnBattleResult) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.route_restriction_enabled) return;
        // is wild battle
        if (*reinterpret_cast<u8*>(ctx->X[19] + 300) == 3) {
            auto battle_result = static_cast<WildBattleResult>(ctx->W[20]);
            // capturing or defeating the wild mon consumes the allowed encounter for that zone
            if (battle_result == WildBattleResult::Capture || battle_result == WildBattleResult::Win) {
                save_file.blacklisted_zones.insert(encounter_zone_hash);
            }
        }
    }
};

HOOK_DEFINE_INLINE(StoreEncounterSpawner) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.route_restriction_enabled) return;
        // TODO: EncountObject struct
        u64 encount_object = ctx->X[23] - 0x50;
        u64 spawner_hash = *reinterpret_cast<u64*>(encount_object + 0xd48);
        auto result = restriction_zone_map.find(spawner_hash);
        if (result != restriction_zone_map.end()) {
            encounter_zone_hash = result->second;
        } else {
            encounter_zone_hash = 0;
        }
    }
};

HOOK_DEFINE_INLINE(StoreFishingPoint) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.route_restriction_enabled) return;
        // TODO: struct
        Field::FieldObject* fishing_point = reinterpret_cast<Field::FieldObject*>(ctx->X[8]);
        auto result = restriction_zone_map.find(fishing_point->unique_hash);
        if (result != restriction_zone_map.end()) {
            encounter_zone_hash = result->second;
        } else {
            encounter_zone_hash = 0;
        }
    }
};

HOOK_DEFINE_INLINE(FilterObject) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.route_restriction_enabled) return;
        auto object = reinterpret_cast<Field::FieldObject*>(ctx->X[0]);
        if (object == nullptr) {
            return;
        }
        auto zone_hash_search = restriction_zone_map.find(object->unique_hash);
        if (zone_hash_search == restriction_zone_map.end()) {
            return;
        }
        if (save_file.blacklisted_zones.find(zone_hash_search->second) != save_file.blacklisted_zones.end()) {
            Field::DeleteFieldObject(object->unique_hash);
        }
    }
};

void install_route_restriction_patch() {
    // TODO: symbol
    // this is the same code that calls the pawn script that
    // handles brilliant aura rewards
    OnBattleResult::InstallAtOffset(0xde77d4 - VER_OFF);
    // TODO: symbol
    // this is where the OverworldSpec is copied from the EncountObject upon encounter
    // covers symbol/hidden/gimmick
    StoreEncounterSpawner::InstallAtOffset(0xdc78e0 - VER_OFF);
    // TODO: symbol
    // this is where the OverworldSpec is copied from the FishingPoint upon encounter
    // covers fishing
    StoreFishingPoint::InstallAtOffset(0xda4d9c - VER_OFF);
    // TODO: symbol
    // reconstructs field object vector from saved data (save file or battle exit)
    FilterObject::InstallAtOffset(0xd18e44 - VER_OFF);
    // TODO: symbol
    // constructs field objects dynamically as they come into load distance
    FilterObject::InstallAtOffset(0xcbb2f8 - VER_OFF);
}