#pragma once
#include <vector>
#include "lib.hpp"

typedef void HIDCallback(HID::HIDData* data);

std::vector<HIDCallback*> hid_callbacks;
u64 old_buttons = 0;

// TODO: other controllers
struct HookNpad : public ::exl::hook::impl::InlineHook<HookNpad> {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto data = reinterpret_cast<HID::HIDData*>(ctx->X[0]);
        for (auto& callback : hid_callbacks) {
            callback(data);
        }
        old_buttons = data->buttons;
    }
};

bool is_any_new(HID::HIDData* data, u64 buttons) {
    return (data->buttons ^ old_buttons) & buttons;
}

bool is_pressed(HID::HIDData* data, u64 buttons) {
    return (data->buttons & buttons) == buttons;
}

bool is_newly_pressed(HID::HIDData* data, u64 buttons) {
    return is_pressed(data, buttons) && is_any_new(data, buttons);
}

void install_hid_patch() {
    HookNpad::InstallAtOffset(HID::PollNpad_offset + 0x84);
}