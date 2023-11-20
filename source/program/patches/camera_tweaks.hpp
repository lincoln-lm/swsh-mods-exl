#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"
#include <math.h>

HOOK_DEFINE_INLINE(PatchCameraConstants) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.camera_tweaks.active) {
            float* camera_obj = reinterpret_cast<float*>(ctx->X[19]);
            camera_obj[0x470/4] = global_config.camera_tweaks.adjustment_speed / 180 * M_PI;
            camera_obj[0x474/4] = global_config.camera_tweaks.max_pitch / 180 * M_PI;
            camera_obj[0x478/4] = global_config.camera_tweaks.min_pitch / 180 * M_PI;
            camera_obj[0x4a0/4] = global_config.camera_tweaks.min_distance;
            camera_obj[0x4c0/4] = global_config.camera_tweaks.max_distance;
        }
    }
};
void install_camera_tweaks_patch() {
    PatchCameraConstants::InstallAtOffset(Camera::Camera_offset + 0x164);
}