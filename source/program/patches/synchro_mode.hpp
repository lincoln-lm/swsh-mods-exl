#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

static bool synchro_mode_active = false;
static Vec3f last_player_position(0);

void synchro_input_callback(HID::HIDData* data) {
    EXL_ASSERT(global_config.initialized);
    if (global_config.synchro_mode.active) {
        if (is_newly_pressed(data, HID::NpadButton::R | HID::NpadButton::L)) {
            auto player = Field::GetPlayerObject();
            synchro_mode_active = !synchro_mode_active;
            if (!synchro_mode_active) {
                reinterpret_cast<bool*>(player)[0x218] = true;
            }
            last_player_position = player->position;
        }
    }
}

HOOK_DEFINE_TRAMPOLINE(OverrideAI) {
    // TODO: Field::EncountObject
    static void Callback(u64 x0, Field::FieldObject* encount_object) {
        EXL_ASSERT(global_config.initialized);
        // is_following_pokemon
        if (global_config.synchro_mode.active && synchro_mode_active && reinterpret_cast<bool*>(encount_object)[0xd58]) {
            auto player = Field::GetPlayerObject();
            auto movement_diff = player->position - last_player_position;
            auto target = encount_object->position + movement_diff;
            reinterpret_cast<bool*>(player)[0x218] = !synchro_mode_active;
            player->position = target;
            last_player_position = target;
            encount_object->rotation = player->rotation;
            if (V3f::magnitude(movement_diff) > 5.0) {
                // TODO: symbol
                // tell the pokemon to run (run=true) to the target pos
                // &encount_object is crudely mimicking a lua PokemonObject
                external<void>(0xe4f550 - VER_OFF, &encount_object, target.x, target.y, target.z, true);
            }
        } else {
            Orig(x0, encount_object);
        }
    };
};
HOOK_DEFINE_TRAMPOLINE(DisableAI) {
    // TODO: Field::EncountObject
    static void Callback(u64 x0, Field::FieldObject* encount_object) {
        EXL_ASSERT(global_config.initialized);
        // is_following_pokemon
        if (synchro_mode_active && reinterpret_cast<bool*>(encount_object)[0xd58]) {
            return;
        } else {
            Orig(x0, encount_object);
        }
    };
};

void install_synchro_mode_patch() {
    hid_callbacks.push_back(synchro_input_callback);
    // TODO: symbols
    DisableAI::InstallAtOffset(0xe51a30 - VER_OFF);
    OverrideAI::InstallAtOffset(0xe519a0 - VER_OFF);
}