#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include <map>

struct fake_gf_string_t {
    u64 unk_0[0x58/8];
    const char16_t* string;
};

static u64 last_hash = 0;
#define ADD_BASIC_MESSAGE(identifier, message) \
    custom_messages[getFNV1aHashedString(identifier).hash] = []() { return u ## message; }
static std::map<u64, std::function<const char16_t* ()>> custom_messages;

HOOK_DEFINE_TRAMPOLINE(MsgStringReplace) {
    static u16 Callback(fake_gf_string_t* dst, fake_gf_string_t* src) {
        fake_gf_string_t replacement;
        auto result = custom_messages.find(last_hash);
        if (result != custom_messages.end()) {
            replacement.string = result->second();
            src = &replacement;
        }
        last_hash = 0;
        return Orig(dst, src);
    }
};

HOOK_DEFINE_TRAMPOLINE(MsgHashStore) {
    static void Callback(u64 param_1, u64* hash_ptr, u64 param_3) {
        last_hash = *hash_ptr;
        if (custom_messages.find(last_hash) != custom_messages.end()) {
            // the rest of the function is problematic (and not neccesary) when run with
            // invalid (custom) hashes
            return;
        }
        return Orig(param_1, hash_ptr, param_3);
    }
};

HOOK_DEFINE_TRAMPOLINE(ValidateMessages) {
    static bool Callback(u64 param_1) {
        return 1;
    }
};

static const u64 addressable_nullptr = 0;

HOOK_DEFINE_INLINE(PatchInvalidMessageNullptr) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        // ValidateMessages allows x8 = nullptr (because the game can't actually
        // find a message corresponding to the hash)
        if (ctx->X[8] == 0) {
            // this is kind of hacky
            // ensure `ldr x22, [x8, #0x60]` sets x22 to 0
            // which triggers a nullptr failsafe & everything works fine
            ctx->X[8] = reinterpret_cast<u64>(&addressable_nullptr) - 0x60;
        }
    }
};

void install_msg_handler_patch() {
    MsgStringReplace::InstallAtOffset(0x67d4d0);
    MsgHashStore::InstallAtOffset(0x67eb10);
    PatchInvalidMessageNullptr::InstallAtOffset(0xeaaef4 - VER_OFF);
    PatchInvalidMessageNullptr::InstallAtOffset(0xeab900 - VER_OFF);
    ValidateMessages::InstallAtOffset(0x13bca40 - VER_OFF);
}