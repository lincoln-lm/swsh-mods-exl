#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"

static bool should_skip = false;

HOOK_DEFINE_TRAMPOLINE(InstantText) {
    static const u32 Callback(void* options_holder) {
        return 3;
    }
};

HOOK_DEFINE_TRAMPOLINE(SkipBSeq) {
    static void Callback(u64 bseq_handler, long param_2) {
        auto gffile = *reinterpret_cast<u64*>(bseq_handler + 0x68);
        if (gffile != 0) {
            const char* file_path = *reinterpret_cast<char**>(gffile + 0x68 + 0x8);
            // skipping the title screen breaks things
            if (strcmp(file_path, "bin/demo/sequence/sd9010_title.bseq")) {
                auto bseq_header = *reinterpret_cast<u32**>(bseq_handler + 0x178);

                bseq_header[3] = 0; // frame count
                bseq_header[4] = 0; // group option count
                bseq_header[5] = 0; // hash size count
            }
        }
        Orig(bseq_handler, param_2);
    }
};

HOOK_DEFINE_TRAMPOLINE(SkipABKeyWait) {
    static s64 Callback(u64 amx) {
        if (!should_skip) {
            return Orig(amx);
        }
        return 0;
    }
};

HOOK_DEFINE_TRAMPOLINE(EvCameraSpeed) {
    static const void Callback(void* options_holder, float* camera_move_settings) {
        if (should_skip) {
            camera_move_settings[15] = 1.0;
        }
        Orig(options_holder, camera_move_settings);
    }
};

HOOK_DEFINE_TRAMPOLINE(InstantMessageEnd) {
    static const u64 Callback(u64 amx) {
        if (!should_skip) {
            return Orig(amx);
        }
        return 1;
    }
};

static void detect_b_held(HID::HIDData* data) {
    should_skip = is_pressed(data, HID::NpadButton::B);
}

void install_speed_qol_patch() {
    // TODO: symbol
    // OptionsHolder::GetTextSpeed
    InstantText::InstallAtOffset(0x140def0);
    // TODO: symbol
    // BSeqHandler::Deserialize
    SkipBSeq::InstallAtOffset(0x14d3960);
    // TODO: symbol
    // ABKeyWait_
    SkipABKeyWait::InstallAtOffset(0x1471900);
    // TODO: symbol
    // IsMsgWinEnd_
    InstantMessageEnd::InstallAtOffset(0x14b5730);
    // TODO: symbol
    // called in EvCameraMove etc. to actually move the camera
    EvCameraSpeed::InstallAtOffset(0xcfde10 - VER_OFF);
    hid_callbacks.push_back(detect_b_held);
}