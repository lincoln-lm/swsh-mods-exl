#pragma once

#include "external.hpp"
#include "schemas/FieldObject_generated.h"

#ifdef VERSION_SHIELD
const u64 VER_OFF = 0;
#else
const u64 VER_OFF = 0x30;
#endif

const u64 SendCommand_offset = 0xea2190 - VER_OFF;
const u64 AuraHandler_offset = 0xdcac10 - VER_OFF;
const u64 FishAuraHandler_offset = 0xd66b20 - VER_OFF;
const u64 MainInit_offset = 0xf112b0 - VER_OFF; // initializes the class holding field objects
const u64 EncountSpawnerInit_offset = 0xdae210 - VER_OFF; // not a ctor
const u64 GetLevelCap_0_offset = 0x13ae400 - VER_OFF;
const u64 GetLevelCap_1_offset = 0x13ae390 - VER_OFF;

namespace Camera {
    const u64 Camera_offset = 0xd3ae00 - VER_OFF;
}

void SendCommand(const char* command) {
    return external<void>(SendCommand_offset, command);
}

namespace GimmickSpawner {
    const u64 GimmickSpawner_offset = 0xd5aad0 - VER_OFF;
}

namespace EncountObject {
    const u64 FromParams_offset = 0xea2670 - VER_OFF;
}

namespace Field {
    const u64 FetchAreaHash_offset = 0xd7e310 - VER_OFF;
    const u64 PushNestHoleEmitter_offset = 0xec5400 - VER_OFF;
    const u64 PushFieldBallItem_offset = 0xd21b20 - VER_OFF;
    const u64 FieldObjects_offset = 0x2955208;

    u64 FetchAreaHash() {
        return external<u64>(FetchAreaHash_offset);
    }
    void* getFieldObjects() {
        return read_main<void*>(FieldObjects_offset);
    }

    template<typename T>
    constexpr u64 getPushOffset() {
        if (std::is_same_v<T, FieldBallItem>) return PushFieldBallItem_offset;
        if (std::is_same_v<T, NestHoleEmitter>) return PushNestHoleEmitter_offset;
        assert(false);
    }
    

    template<typename T>
    u64 PushFieldObject(const T* flatbuffer) {
        return external<u64>(getPushOffset<T>(), getFieldObjects(), flatbuffer);
    }
}

namespace PersonalInfo {
    const u64 FetchInfo_offset = 0x7649f0;
    const u64 GetField_offset = 0x764a10;
    const u64 static_CurrentPersonalInfo_offset = 0x28f5a08;
    enum InfoField {
        FORM_COUNT = 0x1e,
    };
    void FetchInfo(u16 species, u16 form) {
        external<void>(FetchInfo_offset, species, form);
    }
    u32 GetField(InfoField info_field) {
        return external<u32>(GetField_offset, info_field);
    }
    bool IsInGame(u16 species, u16 form) {
        FetchInfo(species, form);
        u8* current_personal_info = read_main<u8*>(static_CurrentPersonalInfo_offset);
        return ((*(current_personal_info + 0x31)) >> 6) & 1;
    }
}

namespace OverworldEncount {
    const u64 GenerateSymbolEncountParam_offset = 0xd050b0 - VER_OFF;
    const u64 FetchSymbolEncountTable_offset = 0xd05750 - VER_OFF;
    const u64 TryGenerateSymbolEncount_offset = 0xdaf380 - VER_OFF;
    const u64 GenerateMainSpec_offset = 0xd311f0 - VER_OFF;
    const u64 GenerateBasicSpec_offset = 0xd32a00 - VER_OFF;
    const u64 InitGimmickSpec_offset = 0xd30d60 - VER_OFF;

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

    void InitGimmickSpec(GimmickSpec *gimmick_spec, OverworldSpec* overworld_spec) {
        external<void>(InitGimmickSpec_offset, gimmick_spec, overworld_spec);
    }
    void GenerateBasicSpec(long param_1, OverworldSpec *overworld_spec, encounter_slot_t *encounter_slot, int minimum_level, int maximum_level, long param_6) {
        external<void>(GenerateBasicSpec_offset, param_1, overworld_spec, encounter_slot, minimum_level, maximum_level);
    }
    bool GenerateSymbolEncountParam(void* param_1, encounter_tables_t* encounter_tables, u64 unused, OverworldSpec* overworld_spec) {
        return external<bool>(GenerateSymbolEncountParam_offset, param_1, encounter_tables, unused, overworld_spec);
    }
    encounter_tables_t FetchSymbolEncountTable(u64 unused, u64* hash) {
        return external<encounter_tables_t>(FetchSymbolEncountTable_offset, unused, hash);
    }
    bool TryGenerateSymbolEncount(EncountSpawner* encount_spawner, void* param_2) {
        return external<bool>(TryGenerateSymbolEncount_offset);
    }
}
