#pragma once

#include "external.hpp"

#ifdef VERSION_SHIELD
static const u64 SendCommand_offset = 0xea2190;
static const u64 AuraHandler_offset = 0xdcac10;
static const u64 FishAuraHandler_offset = 0xd66b20;
static const u64 MainInit_offset = 0xf112b0; // initializes the class holding field objects
#else
static const u64 SendCommand_offset = 0xea2190 - 0x30;
static const u64 AuraHandler_offset = 0xdcac10 - 0x30;
static const u64 FishAuraHandler_offset = 0xd66b20 - 0x30;
static const u64 MainInit_offset = 0xf112b0 - 0x30;
#endif

namespace Camera {
    #ifdef VERSION_SHIELD
    static const u64 Camera_offset = 0xd3ae00;
    #else
    static const u64 Camera_offset = 0xd3ae00 - 0x30;
    #endif
}

static void SendCommand(const char* command) {
    return external<void>(SendCommand_offset, command);
}

namespace EncountObject {
    #ifdef VERSION_SHIELD
    static const u64 FromParams_offset = 0xea2670;
    #else
    static const u64 FromParams_offset = 0xea2670 - 0x30;
    #endif
}

namespace Field {
    #ifdef VERSION_SHIELD
    static const u64 FetchAreaHash_offset = 0xd7e310;
    #else
    static const u64 FetchAreaHash_offset = 0xd7e310 - 0x30;
    #endif

    static u64 FetchAreaHash() {
        return external<u64>(FetchAreaHash_offset);
    }
}

namespace PersonalInfo {
    static const u64 FetchInfo_offset = 0x7649f0;
    static const u64 GetField_offset = 0x764a10;
    static const u64 static_CurrentPersonalInfo_offset = 0x28f5a08;
    enum InfoField {
        FORM_COUNT = 0x1e,
    };
    static void FetchInfo(u16 species, u16 form) {
        external<void>(FetchInfo_offset, species, form);
    }
    static u32 GetField(InfoField info_field) {
        return external<u32>(GetField_offset, info_field);
    }
    // custom function
    static bool IsInGame(u16 species, u16 form) {
        FetchInfo(species, form);
        u8* current_personal_info = read_main<u8*>(static_CurrentPersonalInfo_offset);
        return ((*(current_personal_info + 0x31)) >> 6) & 1;
    }
}

namespace OverworldEncount {
    #ifdef VERSION_SHIELD
    static const u64 GenerateSymbolEncountParam_offset = 0xd050b0;
    static const u64 FetchSymbolEncountTable_offset = 0xd05750;
    static const u64 TryGenerateSymbolEncount_offset = 0xdaf380;
    static const u64 GenerateMainSpec_offset = 0xd311f0;
    static const u64 GenerateBasicSpec_offset = 0xd32a00;
    static const u64 InitGimmickSpec_offset = 0xd30d60;
    #else
    static const u64 GenerateSymbolEncountParam_offset = 0xd050b0 - 0x30;
    static const u64 FetchSymbolEncountTable_offset = 0xd05750 - 0x30;
    static const u64 TryGenerateSymbolEncount_offset = 0xdaf380 - 0x30;
    static const u64 GenerateMainSpec_offset = 0xd311f0 - 0x30;
    static const u64 GenerateBasicSpec_offset = 0xd32a00 - 0x30;
    static const u64 InitGimmickSpec_offset = 0xd30d60 - 0x30;
    #endif

    typedef void EncountSpawner;

    struct OverworldSpec {
        u32 species;
        u16 form;
        u8 level;
        u8 unk_0;
        u32 shininess;
        u16 nature;
        u16 unk_1;
        u32 gender;
        u32 ability;
        u32 has_item;
        u16 held_item;
        u16 unk_2;
        u32 moves[4];
        u32 unk_3;
        u8 unk_4;
        u8 guaranteed_ivs;
        u16 ivs[6];
        u8 unk_5[6]; // evs?
        u16 mark;
        u8 brilliant_index;
        u8 unk_6[5];
        u64 fixed_seed;
    } PACKED;

    struct GimmickSpec {
        u64 unk_0;
        u32 species;
        u16 form;
        u8 unk_1[6];
        u8 level;
        u8 unk_2[3];
        u32 shininess;
        u32 gender;
        u32 nature;
        u32 ability;
        u16 item;
        u8 unk_3[6];
        u64 unk_4[2];
        u8 unk_5[4];
        u32 moves[4];
        u8 unk_6[3];
        u8 ivs[6];
        u8 unk_7[6]; // evs?
    } PACKED;

    struct encounter_slot_t {
        u16 rate;
        u16 species;
        u16 form;
    } PACKED;

    struct encounter_tables_t {
        u8 data[566];
    } PACKED;

    static void InitGimmickSpec(GimmickSpec *gimmick_spec, OverworldSpec* overworld_spec) {
        external<void>(InitGimmickSpec_offset, gimmick_spec, overworld_spec);
    }
    static void GenerateBasicSpec(long param_1, OverworldSpec *overworld_spec, encounter_slot_t *encounter_slot, int minimum_level, int maximum_level, long param_6) {
        external<void>(GenerateBasicSpec_offset, param_1, overworld_spec, encounter_slot, minimum_level, maximum_level);
    }
    static bool GenerateSymbolEncountParam(void* param_1, encounter_tables_t* encounter_tables, u64 unused, OverworldSpec* overworld_spec) {
        return external<bool>(GenerateSymbolEncountParam_offset, param_1, encounter_tables, unused, overworld_spec);
    }
    static encounter_tables_t FetchSymbolEncountTable(u64 unused, u64* hash) {
        return external<encounter_tables_t>(FetchSymbolEncountTable_offset, unused, hash);
    }
    static bool TryGenerateSymbolEncount(EncountSpawner* encount_spawner, void* param_2) {
        return external<bool>(TryGenerateSymbolEncount_offset);
    }
}
