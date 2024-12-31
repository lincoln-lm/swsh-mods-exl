#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

#include <unordered_map>
using namespace Field;

std::unordered_map<u64, hashed_string_t> file_list;

// TODO: doc & clean
bool handleRender(UnitObject* unit_obj, u64 param_2, bool param_3) {
    unit_obj->is_rendered = true;

    auto file_obj = unit_obj->allocator->Alloc(0x1B0, 0x10);
    memset(file_obj, 0, 0x1B0);
    *reinterpret_cast<uintptr_t*>(file_obj) = main_offset(0x2568b00);
    *reinterpret_cast<uintptr_t*>(file_obj+0x10) = main_offset(0x2530f30);
    // some heap object
    struct holder_t {
        void* file_obj;
        Allocator* allocator;
        u64 size;
        u64 align;
    };
    holder_t holder = {
        file_obj,
        unit_obj->allocator,
        0x1B0,
        0x10
    };
    *reinterpret_cast<holder_t*>(unit_obj+0x3d0) = holder;

    *reinterpret_cast<UnitObject**>(file_obj+0x150) = unit_obj;
    *reinterpret_cast<u64*>(file_obj+0x158) = param_2;
    *reinterpret_cast<uintptr_t*>(file_obj+0x130) = main_offset(0xcc29d0 - VER_OFF);
    *reinterpret_cast<uintptr_t*>(file_obj+0x138) = main_offset(0xcc2a30 - VER_OFF);
    *reinterpret_cast<uintptr_t*>(file_obj+0x140) = main_offset(0xcc2a40 - VER_OFF);
    *reinterpret_cast<uintptr_t*>(file_obj+0x148) = main_offset(0xcc2a50 - VER_OFF);

    *reinterpret_cast<void**>(file_obj+0x40) = file_obj;
    *reinterpret_cast<uintptr_t*>(file_obj+0x20) = main_offset(0xf0db90) - VER_OFF;
    *reinterpret_cast<uintptr_t*>(file_obj+0x28) = main_offset(0xf0dc50) - VER_OFF;
    *reinterpret_cast<uintptr_t*>(file_obj+0x30) = main_offset(0xf0dc60) - VER_OFF;
    *reinterpret_cast<uintptr_t*>(file_obj+0x38) = main_offset(0xf0dc70) - VER_OFF;
    auto gfpak_path = file_list[unit_obj->primary_model_path.hash];
    *reinterpret_cast<void**>(file_obj+0x8) = unit_obj->allocator;
    struct {
        u64 hash;
        hashed_string_t path;
        u32 drive;
    } PACKED file_handle;
    file_handle.path.unk = 0;
    // absolute path
    external<void>(0xc99c40 - VER_OFF, &file_handle, &gfpak_path.string, 1);
    // hacky: pad to >128 to trigger return storage
    struct unk_holder_t {
        u64 unk_0;
        u64 unk_1;
        u64 unk_2;
    };
    unk_holder_t unk = external<unk_holder_t>(0x5dccd0 - VER_OFF, read_main<u64>(0x26364e0), 1, &file_handle, 0, param_2, 0);
    *reinterpret_cast<u64*>(file_obj+0x18) = unk.unk_0;
    if (unk.unk_0 != 0) {
        auto sub_unk = unk.unk_0 + 0x48;
        external_absolute<void>(*(*reinterpret_cast<u64**>(sub_unk) + 2), sub_unk);
        external<void>(0x5e1e70 - VER_OFF, unk.unk_0, file_obj+0x10, 1);
    }

    external<void>(0xcbd350 - VER_OFF, unit_obj, param_2, param_3);
    return true;
}
// builds UnitObject with replaced render function
UnitObject* pushCustomUnitObject(const FlatbufferObjects::UnitObject* flatbuffer) {
    static void* vtable[71] = { 0 };
    auto unit_obj = reinterpret_cast<UnitObject*>(pushFieldObject(flatbuffer));
    void** vtable_ptr = reinterpret_cast<void**>(unit_obj);
    memcpy(vtable, *vtable_ptr, sizeof(vtable));
    vtable[0x50/8] = reinterpret_cast<void*>(handleRender);
    *vtable_ptr = vtable;
    return unit_obj;
}

std::vector<UnitObject*> objs;
void NewModel(Vec4f pos, const char* gfpak, const char* mdl) {
    flatbuffers::FlatBufferBuilder builder;
    builder.Finish(
        FlatbufferObjects::CreateUnitObject(
            builder,
            FlatbufferObjects::CreateUnitObjectInner(
                builder,
                FlatbufferObjects::CreateFieldObject(
                    builder,
                    pos.x,
                    pos.y,
                    pos.z,
                    0,
                    0,
                    0,
                    1,
                    1,
                    1
                )
            )
        )
    );
    UnitObject* obj = reinterpret_cast<UnitObject*>(pushCustomUnitObject(flatbuffers::GetRoot<FlatbufferObjects::UnitObject>(builder.GetBufferPointer())));
    obj->primary_model_path = getFNV1aHashedString(mdl);
    file_list[obj->primary_model_path.hash] = getFNV1aHashedString(gfpak);
    obj->AddModel(&obj->primary_model_path, 0);
    objs.push_back(obj);
    external<void>(0xcba890 - VER_OFF, getFieldSingleton(), 1);
}

FieldObject* NewPokemonModel(Vec4f pos, float scale, u16 species, u16 form, u16 gender, bool is_shiny, u64 script_id) {
    flatbuffers::FlatBufferBuilder builder;
    builder.Finish(
        FlatbufferObjects::CreatePokemonModel(
            builder,
            0,
            FlatbufferObjects::CreatePokemonModelInner(
                builder,
                FlatbufferObjects::CreatePokemonModelInnerInner(
                    builder,
                    FlatbufferObjects::CreateFieldObject(
                        builder,
                        pos.x,
                        pos.y,
                        pos.z,
                        0,
                        0,
                        0,
                        scale,
                        scale,
                        scale,
                        0xabcd,
                        0xabcd,
                        script_id
                    ),
                    0xcbf29ce484222645Ul,
                    0xcbf29ce484222645Ul,
                    0xcbf29ce484222645Ul,
                    FlatbufferObjects::CreatePokemonModelInnerInnerInner(
                        builder,
                        1,
                        0,
                        50,
                        0,
                        45,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0
                    ),
                    true,
                    0xcbf29ce484222645Ul,
                    FlatbufferObjects::CreatePokemonModelInnerInnerInner(
                        builder,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0
                    )
                )
            ),
            species,
            form,
            gender,
            is_shiny,
            1,
            0xcbf29ce484222645Ul,
            0xcbf29ce484222645Ul,
            0xcbf29ce484222645Ul,
            builder.CreateVector<u16>({}),
            1.0,
            FlatbufferObjects::CreatePokemonModelUnk(
                builder,
                false,
                false,
                false,
                0,
                0xcbf29ce484222645Ul,
                false,
                false,
                0xcbf29ce484222645Ul,
                3,
                0
            ),
            0,
            14,
            true
        )
    );
    return reinterpret_cast<FieldObject*>(pushFieldObject(flatbuffers::GetRoot<FlatbufferObjects::PokemonModel>(builder.GetBufferPointer())));
}

void debug_input_callback(HID::HIDData* data) {
    EXL_ASSERT(global_config.initialized);
    if (is_newly_pressed(data, HID::NpadButton::R | HID::NpadButton::A)) {
        AMX::show_custom_message(u"\023R+A pressed\023");
        auto player = GetPlayerObject();
        NewPokemonModel(player->position, 2.0, 132, 0, 0, true, getFNV1aHashedString("debug_msg_1").hash);
        external<void>(0xcba890 - VER_OFF, getFieldSingleton(), 1);
        // NewModel(player->position, "sd:/pm0840_00.gfpak", "bin/pokemon/pm0840_00/mdl/pm0840_00_rare.gfbmdl");
    }
}

HOOK_DEFINE_INLINE(DumpFlatbuffer) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        external<void>(0x18fe1d0, FlatbufferObjects::ObjectToString(reinterpret_cast<FlatbufferObjects::GimmickEncountSpawner*>(ctx->X[3])).c_str());
    };
};

HOOK_DEFINE_INLINE(DebugTick) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {};
};

void install_debug_patch() {
    hid_callbacks.push_back(debug_input_callback);
    DebugTick::InstallAtOffset(0xcba730 - VER_OFF);
    DumpFlatbuffer::InstallAtOffset(GimmickSpawner::GimmickSpawner_offset);
}