#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"
#include <math.h>

HOOK_DEFINE_INLINE(PatchCameraConstants) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.camera_tweaks.active) {
            auto camera_obj = reinterpret_cast<Field::ExtendedCamera*>(ctx->X[19]);
            camera_obj->camera_speed = global_config.camera_tweaks.adjustment_speed / 180 * M_PI;
            camera_obj->maximum_pitch = global_config.camera_tweaks.max_pitch / 180 * M_PI;
            camera_obj->minimum_pitch = global_config.camera_tweaks.min_pitch / 180 * M_PI;
            camera_obj->minimum_distance = global_config.camera_tweaks.min_distance;
            camera_obj->maximum_distance = global_config.camera_tweaks.max_distance;
        }
    }
};

HOOK_DEFINE_INLINE(DisableDLCMaxPitch) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.camera_tweaks.active) {
            auto camera_obj = reinterpret_cast<Field::ExtendedCamera*>(ctx->X[21]);
            // DLC areas set this to 0 entirely seperately for some reason
            camera_obj->maximum_pitch = global_config.camera_tweaks.max_pitch / 180 * M_PI;
        }
    }
};

void install_camera_tweaks_patch() {
    PatchCameraConstants::InstallAtOffset(Field::ExtendedCamera::ExtendedCamera_offset + 0x164);
    // TODO: symbol
    DisableDLCMaxPitch::InstallAtOffset(0xd19c98 - VER_OFF);
}