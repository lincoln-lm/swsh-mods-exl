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
const u64 EulerToQuaternion_offset = 0x992cd0 - VER_OFF;
const u64 QuaternionToEuler_offset = 0x6101c0;

// to use the q/v registers (128-bit float/float vector)
// using f128 is often required
union Vec4f {
    f128 q;
    f64 d[2];
    f32 f[4];
    struct {
        float x;
        float y;
        float z;
        float w;
    } quat;
    struct {
        float pitch;
        float yaw;
        float roll;
        float _;
    } euler;

    Vec4f() {}
    Vec4f(f128 _q) : q(_q) {}
};

Vec4f QuaternionToEuler(Vec4f* q) {
    return external<f128>(QuaternionToEuler_offset, q);
}

Vec4f EulerToQuaternion(float yaw, float pitch, float roll) {
    return external<f128>(EulerToQuaternion_offset, yaw, pitch, roll);
}

namespace HID {
    const u64 PollNpad_offset = 0xf1f9d0 - VER_OFF;

    enum NpadButton {
        A = 1 << 0,
        B = 1 << 1,
        X = 1 << 2,
        Y = 1 << 3,
        StickL = 1 << 4,
        StickR = 1 << 5,
        L = 1 << 6,
        R = 1 << 7,
        ZL = 1 << 8,
        ZR = 1 << 9,
        Plus = 1 << 10,
        Minus = 1 << 11,
        Left = 1 << 12,
        Up = 1 << 13,
        Right = 1 << 14,
        Down = 1 << 15,
        StickLLeft = 1 << 16,
        StickLUp = 1 << 17,
        StickLRight = 1 << 18,
        StickLDown = 1 << 19,
        StickRLeft = 1 << 20,
        StickRUp = 1 << 21,
        StickRRight = 1 << 22,
        StickRDown = 1 << 23,
        LeftSL = 1 << 24,
        LeftSR = 1 << 25,
        RightSL = 1 << 26,
        RightSR = 1 << 27,
        Palma = 1 << 28,
        // other
    };

    struct HIDData {
        u64 unk_0;
        u64 buttons;
        // TODO: when is this actually set properly?
        u64 old_buttons;
        struct {
            float x;
            float y;
        } stick_r, stick_l;
        u32 unk_1;
        u32 unk_2;
        u32 unk_3;
        u8 unk_4;
    } PACKED;

    void PollNpad(HIDData* data) {
        return external<void>(PollNpad_offset, data);
    }
}

namespace Camera {
    const u64 Camera_offset = 0xd3ae00 - VER_OFF;
    const u64 GetInheritance_offset = 0xd3e2f0 - VER_OFF;

    void* GetInheritance() {
        return external<void*>(GetInheritance_offset);
    }
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
    const u64 GetPlayerObject_offset = 0xd7e290 - VER_OFF;
    const u64 PushNestHoleEmitter_offset = 0xec5400 - VER_OFF;
    const u64 PushFieldBallItem_offset = 0xd21b20 - VER_OFF;
    const u64 FieldObjects_offset = 0x2955208;

    u64 FetchAreaHash() {
        return external<u64>(FetchAreaHash_offset);
    }
    u64 GetPlayerObject() {
        return external<u64>(GetPlayerObject_offset);
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
