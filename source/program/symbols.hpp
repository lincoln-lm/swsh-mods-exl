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
const u64 EncountObject_offset = 0xd5da60 - VER_OFF;
const u64 FishingPoint_offset = 0xd665c0 - VER_OFF;
// TODO: handles a few more things other than just aura
const u64 UpdatesAura_offset = 0xd5fa80 - VER_OFF;
const u64 FishAuraHandler_offset = 0xd66b20 - VER_OFF;
const u64 MainInit_offset = 0xf112b0 - VER_OFF; // initializes the class holding field objects
const u64 EncountSpawnerInit_offset = 0xdae210 - VER_OFF; // not a ctor
const u64 GetLevelCap_0_offset = 0x13ae400 - VER_OFF;
const u64 GetLevelCap_1_offset = 0x13ae390 - VER_OFF;
const u64 EulerToQuaternion_offset = 0x992cd0 - VER_OFF;
const u64 QuaternionToEuler_offset = 0x6101c0;
const u64 SetUpBattleEnvironment_offset = 0x968b70;
const u64 InitSoundEngine_offset = 0x7937f0;
const u64 LoadSoundBank_offset = 0x795650;
const u64 GetSpeciesFormIndex_offset = 0x77f600;
const u64 RestorePartyOrder_offset = 0x7df560;
const u64 LoadTrainerPokeData_offset = 0x1522b30 - VER_OFF; 

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
    };
    struct {
        float pitch;
        float yaw;
        float roll;
        float _;
    };

    Vec4f(f128 _q) : q(_q) {}
} PACKED;

// padded to 16 bytes anyway
typedef Vec4f Vec3f;

struct hashed_string_t {
    u64 hash;
    const char* string;
    u64 length;
    u64 unk;
} PACKED;

Vec3f QuaternionToEuler(Vec4f* q) {
    return external<f128>(QuaternionToEuler_offset, q);
}

Vec4f EulerToQuaternion(float yaw, float pitch, float roll) {
    return external<f128>(EulerToQuaternion_offset, yaw, pitch, roll);
}

struct MoveIdHolder {
    static const u64 IsMoveUsable_offset = 0x7814a0;
    bool IsMoveUsable() {
        return external<bool>(IsMoveUsable_offset, this);
    }
    u64 unk;
    u32 move_id;
} PACKED;

struct learnset_item_t {
    s16 move_id;
    s16 level;
} PACKED;

struct LearnsetData {
    static const u64 FetchLearnsetData_offset = 0x785df0;
    static const u64 WAZAOBOE_TOTAL_LEARNSET_DATA_offset = 0x292cff0;
    void FetchLearnsetData(u32 species, u16 form) {
        return external<void>(FetchLearnsetData_offset, this, species, form);
    }
    static learnset_item_t (*WAZAOBOE_TOTAL_LEARNSET_DATA())[65] {
        return read_main<learnset_item_t (*)[65]>(LearnsetData::WAZAOBOE_TOTAL_LEARNSET_DATA_offset);
    }
    u64 vtable;
    u32 species;
    u16 form;
    learnset_item_t learnset_items[65];
    u8 count;
} PACKED;

struct party_member_t {
    u8 unk_0[0x74];
    u32 hp;
    u8 unk_1[0x5];
    u8 real_party_index;
    // unknown size
} PACKED;

struct base_party_t {
    party_member_t *members[6];
    u8 count;
} PACKED;

struct trainer_poke_data_t {
    u8 gender : 2;
    u8 unk_0 : 2;
    u8 ability : 2;
    u8 unk_1 : 2;
    u8 nature;
    u8 ev_hp;
    u8 ev_atk;
    u8 ev_def;
    u8 ev_spa;
    u8 ev_spd;
    u8 ev_spe;
    u8 dynamax_level;
    u8 can_gigantamax;
    u16 level;
    u16 species;
    u16 form;
    u16 held_item;
    s16 moves[4];
    u16 unk_2;
    u32 iv_hp : 5;
    u32 iv_atk : 5;
    u32 iv_def : 5;
    u32 iv_spa : 5;
    u32 iv_spd : 5;
    u32 iv_spe : 5;
    bool shiny : 1;
    bool can_dynamax : 1;
};
static_assert(sizeof(trainer_poke_data_t) == 0x20);

namespace AMX {
    const u64 CallPawnScript_offset = 0x14c9aa0 - VER_OFF;
    const u64 PG_WordSetRegister_offset = 0x14b6690 - VER_OFF;
    const u64 WordSetRegister_PlayerName_offset = 0x13c9060 - VER_OFF;
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

void SendCommand(const char* command) {
    return external<void>(SendCommand_offset, command);
}

void LoadSoundBank(u64 unk0, hashed_string_t* name, u64 unk1, u64 unk2, u64 unk3) {
    return external<void>(LoadSoundBank_offset, unk0, name, unk1, unk2, unk3);
}

u64 GetSpeciesFormIndex(u64 species, u32 form) {
    return external<u64>(GetSpeciesFormIndex_offset, species, form);
}

namespace GimmickSpawner {
    const u64 GimmickSpawner_offset = 0xd5aad0 - VER_OFF;
}

namespace EncountObject {
    const u64 FromParams_offset = 0xea2670 - VER_OFF;
}

// TODO: fill out
struct Allocator {
    virtual ~Allocator();
    virtual void* Alloc(u64 size, u64 align);
};

// TODO: multiple inheritance shenanigans
struct BaseObject
{
    static const u64 vtable_offset = 0x2508468;
    virtual ~BaseObject();
    virtual int func_0x10();
    virtual int func_0x18();
    virtual int func_0x20();
    virtual int func_0x28();

    Allocator* allocator;
    u8 unk_0[0x8];
    u64 size;
    u64 align;
    u8 unk_1[0x28];
    // main+offset to a function returning the object's inheritance pointer
    u64* inheritance_function;
    u64 unk_2;
} PACKED;
static_assert(sizeof(BaseObject) == 0x60);

struct WorldObject : public BaseObject
{
    static const u64 vtable_offset = 0x250a0b0;
    virtual void* GetInstanceInheritance() const;
    // defaults to identity
    f32 some_matrix[4][4];
    Vec4f rotation;
    Vec3f position;
    bool position_modified;
    u8 _padding[3];
} PACKED;

struct ScaledWorldObject : public WorldObject
{
    static const u64 vtable_offset = 0x250aa78;
    f32 unk_3[3];
    Vec3f scale;
    u8 unk_5[0xB0];
} PACKED;
static_assert(sizeof(ScaledWorldObject) == 0x190);

namespace Field {
    const u64 FetchZoneHash_offset = 0xd7e310 - VER_OFF;
    const u64 GetPlayerObject_offset = 0xd7e290 - VER_OFF;
    const u64 PushNestHoleEmitter_offset = 0xec5400 - VER_OFF;
    const u64 PushFieldBallItem_offset = 0xd21b20 - VER_OFF;
    const u64 PushUnitObject_offset = 0xd25810 - VER_OFF;
    const u64 PushPokemonModel_offset = 0xd204f0 - VER_OFF;
    const u64 PushGimmickEncountSpawner_offset = 0xd2a6e0 - VER_OFF;
    const u64 DeleteFieldObject_offset = 0xd774e0 - VER_OFF;
    const u64 FieldSingleton_offset = 0x2955208;

    struct FieldObject : public ScaledWorldObject
    {
        static const u64 vtable_offset = 0x254f888;
        virtual int func_0x38();
        virtual int OnDespawn();
        virtual int func_0x48();
        virtual bool HandleRender(u64 param_2, bool value);
        virtual int func_0x58();
        virtual int func_0x60();
        virtual int func_0x68();
        virtual int func_0x70();
        virtual int func_0x78();
        virtual int func_0x80();
        virtual int func_0x88();
        virtual int OnTick();
        virtual int func_0x98();
        virtual int func_0xa0();
        virtual int func_0xa8();
        virtual int func_0xb0();
        virtual int func_0xb8();
        virtual int func_0xc0();
        virtual int func_0xc8();
        virtual int func_0xd0();
        virtual int func_0xd8();
        virtual int func_0xe0();
        virtual int func_0xe8();
        virtual int OnLoad();
        virtual int func_0xf8();
        virtual int func_0x100();
        virtual int func_0x108();
        virtual int func_0x110();
        virtual int func_0x118();
        virtual int func_0x120();
        virtual int func_0x128();
        virtual int func_0x130();
        virtual int func_0x138();
        virtual int func_0x140();
        virtual int func_0x148();
        virtual int func_0x150();
        virtual int func_0x158();
        virtual int func_0x160();
        virtual int func_0x168();
        virtual int func_0x170();

        u8 unk_6[0x10];
        u64 unique_hash;
        u8 unk_7[0x1C];
        bool is_rendered;
        u8 unk_88[0x53];
        bool is_visible;
        bool is_culling;
        u8 unk_8[0x17E];
    } PACKED;
    static_assert(sizeof(FieldObject) == 0x398);

    struct EncountSpawner : FieldObject {
        static const u64 vtable_offset = 0x255ee90;
        u8 unk_9[0xB0];
        s32 maximum_symbol_encounts;
        u8 unk_10[0x2D4];

        static const u64 EncountSpawner_offset = 0xdae0b0 - VER_OFF;
    } PACKED;
    static_assert(sizeof(EncountSpawner) == 0x720);

    struct Camera : FieldObject {
        static const u64 vtable_offset = 0x2550f30;
        virtual int func_0x178();
        virtual void SetCameraInInUse(bool value);
        virtual bool GetCameraIsInUse();
        u8 unk_9[0x3C];
        float pitch;
        float unk_91[3];
    } PACKED;
    static_assert(sizeof(Camera) == 0x3E4);

    struct ModelObject : FieldObject {
        static const u64 vtable_offset = 0x254fd18;
        // definitely bigger
        u8 unk_9[0x58];
        hashed_string_t primary_model_path;
        u8 unk_10[0xad];
        static const u64 UnitObject_offset = 0xcbc9d0 - VER_OFF;
    } PACKED;
    static_assert(sizeof(ModelObject) == 0x4BD);
    struct UnitObject : ModelObject {
        static const u64 vtable_offset = 0x255e658;
        void AddModel(const hashed_string_t* path, u64 param_3) {
            external<void>(AddModel_offset, this, path, param_3);
        }
        void AddAnimation(const hashed_string_t* path, u64 param_3) {
            external<void>(AddAnimation_offset, this, path, param_3);
        }
        static const u64 AddModel_offset = 0xcd8670;
        static const u64 AddAnimation_offset = 0xcda070;
    } PACKED;
    static_assert(sizeof(UnitObject) == 0x4BD);

    struct ExtendedCamera : Camera {
        static const u64 vtable_offset = 0x25573d0;

        u8 unk_10[0x1C];
        float distance_from_player;
        u8 unk_11[0x1C];
        float toggle_distance_0;
        u8 unk_12[0x1C];
        float toggle_distance_1;
        u8 unk_13[0x2C];
        float camera_speed;
        float maximum_pitch;
        float minimum_pitch;
        u8 unk_14[0x24];
        float minimum_distance;
        u8 unk_15[0x1C];
        float maximum_distance;
        u8 unk_16[0x17C];

        static const u64 ExtendedCamera_offset = 0xd3ae00 - VER_OFF;
    } PACKED;
    static_assert(sizeof(ExtendedCamera) == 0x640);

    // TODO: naming
    struct FieldSingleton {
        u8 unk_0[0xb0];
        std::vector<FieldObject*> field_objects;
        u64 unk_1;
    };
    static_assert(sizeof(FieldSingleton) == 0xd0);

    u64 FetchZoneHash() {
        return external<u64>(FetchZoneHash_offset);
    }
    // TODO: PlayerObject
    FieldObject* GetPlayerObject() {
        return external<FieldObject*>(GetPlayerObject_offset);
    }
    bool DeleteFieldObject(u64 object_hash) {
        return external<bool>(DeleteFieldObject_offset, &object_hash);
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
        u8 minimum_level;
        u8 maximum_level;
        encounter_slot_t encounter_slots[10];
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
    bool TryGenerateSymbolEncount(Field::EncountSpawner* encount_spawner, void* param_2) {
        return external<bool>(TryGenerateSymbolEncount_offset);
    }
}
