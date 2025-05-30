#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"

HOOK_DEFINE_TRAMPOLINE(InstantText) {
    static const u32 Callback(void* options_holder) {
        return 3;
    }
};


void install_instant_text_patch() {
    // TODO: symbol
    // OptionsHolder::GetTextSpeed
    InstantText::InstallAtOffset(0x140def0);
}