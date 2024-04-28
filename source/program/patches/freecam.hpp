#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"
#include <cmath>

bool is_freecam = false;
bool directions[4] = {false, false, false, false};
f32 speed = 128.0;
int vertical_direction = 0;

u64 get_camera() {
    auto current_ptr = *reinterpret_cast<u64**>(Field::getFieldObjects() + 0xb0);
    auto end = *reinterpret_cast<u64**>(Field::getFieldObjects() + 0xb8);
    for (; current_ptr < end; current_ptr++) {
        auto obj = *current_ptr;
        if (Field::checkInheritance(obj, Camera::GetInheritance())) {
            return obj;
        };
    }
    return 0;
}

HOOK_DEFINE_INLINE(Tick) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        auto camera = get_camera();
        if (is_freecam && camera) {
            auto camera_pos = reinterpret_cast<Vec4f*>(camera + 0xb0);
            auto camera_dir = QuaternionToEuler(reinterpret_cast<Vec4f*>(camera + 0xa0)).euler.yaw;
            for (int i = 0; i < 4; i++) {
                if (directions[i]) {
                    camera_pos->quat.x += sin(camera_dir + M_PI_2 * i) * speed;
                    camera_pos->quat.z += cos(camera_dir + M_PI_2 * i) * speed;
                }
            }
            camera_pos->quat.y += vertical_direction * speed;
        }
    }
};

// TODO: very hacky
u128 pos;
HOOK_DEFINE_INLINE(StorePos1) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.freecam.active) {
            pos = *reinterpret_cast<u128*>(ctx->X[20]+0xb0);
        }
    }
};
HOOK_DEFINE_INLINE(SetPos1) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.freecam.active) {
            if (is_freecam) *reinterpret_cast<u128*>(ctx->X[20]+0xb0) = pos;
        }
    }
};
HOOK_DEFINE_INLINE(StorePos2) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.freecam.active) {
            pos = *reinterpret_cast<u128*>(ctx->X[19]+0xb0);
        }
    }
};
HOOK_DEFINE_INLINE(SetPos2) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.freecam.active) {
            if (is_freecam) *reinterpret_cast<u128*>(ctx->X[19]+0xb0) = pos;
        }
    }
};

void input_callback(HID::HIDData* data) {
    EXL_ASSERT(global_config.initialized);
    if (global_config.freecam.active) {
        if (is_newly_pressed(data, HID::NpadButton::R | HID::NpadButton::A)) {
            is_freecam = !is_freecam;
        } else if (is_freecam) {
            if (is_newly_pressed(data, HID::NpadButton::L)) {
                speed /= 1.5f;
            } else if (is_newly_pressed(data, HID::NpadButton::R)) {
                speed *= 1.5f;
            }
        }
        directions[0] = is_pressed(data, HID::NpadButton::Down);
        directions[1] = is_pressed(data, HID::NpadButton::Right);
        directions[2] = is_pressed(data, HID::NpadButton::Up);
        directions[3] = is_pressed(data, HID::NpadButton::Left);
        vertical_direction = is_pressed(data, HID::NpadButton::ZL) ? -1 : is_pressed(data, HID::NpadButton::ZR) ? 1 : 0;
    }
}


void install_freecam_patch() {
    hid_callbacks.push_back(input_callback);
    // TODO: symbols
    Tick::InstallAtOffset(0xcba730 - VER_OFF);
    StorePos1::InstallAtOffset(0x60f5a0);
    SetPos1::InstallAtOffset(0x60f5a4);
    StorePos2::InstallAtOffset(0xd3c36c - VER_OFF);
    SetPos2::InstallAtOffset(0xd3c370 - VER_OFF);
}