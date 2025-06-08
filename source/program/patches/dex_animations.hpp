#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

struct animation_entry_t {
    u64 unk_0;
    hashed_string_t animation_string;
    u64 unk_1;
} PACKED;
static_assert(sizeof(animation_entry_t) == 0x30);

static u64 current_animation = 0;

HOOK_DEFINE_INLINE(SwapAnimation) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (!global_config.dex_animations.active) return;
        if (current_animation == 0) return;
        auto target_animation = reinterpret_cast<hashed_string_t*>(ctx->X[1]);
        target_animation->hash = current_animation;
    }
};


HOOK_DEFINE_INLINE(SpawnTrigger) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (!global_config.dex_animations.active) return;
        static bool spawned = false;
        if (spawned) return;
        spawned = true;
        auto player = Field::GetPlayerObject();
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
                            player->position.x,
                            player->position.y,
                            player->position.z,
                            0,
                            0,
                            0,
                            1.0,
                            1.0,
                            1.0,
                            0xabcd,
                            0xabcd,
                            getFNV1aHashedString("dex_dialog").hash
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
                132,
                0,
                0,
                false,
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
        Field::pushFieldObject(flatbuffers::GetRoot<FlatbufferObjects::PokemonModel>(builder.GetBufferPointer()));
        external<void>(0xcba890 - VER_OFF, Field::getFieldSingleton(), 1);
    }
};

u64 SetAnimation_(void* amx, u64* params) {
    current_animation = params[1];
    return 1;
}


void install_dex_animations_patch() {
    SpawnTrigger::InstallAtOffset(0x14c8ac0 - VER_OFF);
    ADD_BASIC_MESSAGE("", "");
    ADD_BASIC_MESSAGE("ask_animation", "Select an animation");
    ADD_BASIC_MESSAGE("option_back", "Back");
    ADD_BASIC_MESSAGE("option_next", "Next");
    ADD_BASIC_MESSAGE("to_ba01_landC01", "to_ba01_landC01");
    ADD_BASIC_MESSAGE("to_ba01_land_state", "to_ba01_land_state");
    ADD_BASIC_MESSAGE("to_ba02_megaappeal01", "to_ba02_megaappeal01");
    ADD_BASIC_MESSAGE("to_ba02_roar01", "to_ba02_roar01");
    ADD_BASIC_MESSAGE("to_ba10_waitA01", "to_ba10_waitA01");
    ADD_BASIC_MESSAGE("to_ba10_waitB01", "to_ba10_waitB01");
    ADD_BASIC_MESSAGE("to_ba20_buturi01", "to_ba20_buturi01");
    ADD_BASIC_MESSAGE("to_ba20_buturi02", "to_ba20_buturi02");
    ADD_BASIC_MESSAGE("to_ba20_buturi03", "to_ba20_buturi03");
    ADD_BASIC_MESSAGE("to_ba20_buturi04", "to_ba20_buturi04");
    ADD_BASIC_MESSAGE("to_ba21_tokusyu01", "to_ba21_tokusyu01");
    ADD_BASIC_MESSAGE("to_ba21_tokusyu02", "to_ba21_tokusyu02");
    ADD_BASIC_MESSAGE("to_ba21_tokusyu03", "to_ba21_tokusyu03");
    ADD_BASIC_MESSAGE("to_ba21_tokusyu04", "to_ba21_tokusyu04");
    ADD_BASIC_MESSAGE("to_ba30_damageS01", "to_ba30_damageS01");
    ADD_BASIC_MESSAGE("to_ba41_down01", "to_ba41_down01");
    ADD_BASIC_MESSAGE("to_ba50_wideuse01", "to_ba50_wideuse01");
    ADD_BASIC_MESSAGE("to_ba50_wideuse02", "to_ba50_wideuse02");
    ADD_BASIC_MESSAGE("to_ba50_wideuse03", "to_ba50_wideuse03");
    ADD_BASIC_MESSAGE("to_eye_blink01", "to_eye_blink01");
    ADD_BASIC_MESSAGE("to_eye_blink02", "to_eye_blink02");
    ADD_BASIC_MESSAGE("to_eye_blink03", "to_eye_blink03");
    SwapAnimation::InstallAtOffset(0x5b1f30);
    AMX::add_new_symbol("SetAnimation_", SetAnimation_);
}