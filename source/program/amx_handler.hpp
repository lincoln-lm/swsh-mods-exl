#pragma once
#include "hook/inline.hpp"
#include "lib.hpp"
#include "types.h"
#include "util.hpp"


namespace AMX {
    struct AMX_INSTANCE {
        void* unk_0[6];
        u64 cip;
        u64 frm;
        u64 hea;
        u64 hlw;
        u64 stk;
        u64 stp;
        // ...
    };
    struct AMX_HEADER {
        u32 size;
        u16 magic;
        u8 file_version;
        u8 amx_version;
        u16 flags;
        u16 defsize;
        u32 cod;
        u32 dat;
        u32 hea;
        u32 stp;
        u32 cip;
        // ...
    };
    struct Symbol {
        const char* name;
        u64 (*function)(void*, u64*);
    };
    static inline std::vector<Symbol> custom_symbols = {
        {nullptr, nullptr}
    };
    typedef void (*amx_callback)(AMX_INSTANCE*, u64*);
    static inline std::vector<amx_callback> amx_callbacks = {};
    void add_new_symbol(const char* name, u64 (*function)(void*, u64*)) {
        AMX::custom_symbols.emplace(AMX::custom_symbols.end() - 1, name, function);
    }
    void add_amx_callback(amx_callback callback) {
        AMX::amx_callbacks.emplace_back(callback);
    }
    u64 call_pawn_script(const char* script_id) {
        // hacky: pad to >128 to trigger return storage
        struct unk_holder_t {
            u64 result_value;
            u64 unk_1;
            u64 unk_2;
        };
        unk_holder_t result = external<unk_holder_t>(CallPawnScript_offset, getFNV1aHashedString(script_id).hash, 0xabcd, 0, 0, 0, 0);
        return result.result_value;
    }
}

HOOK_DEFINE_TRAMPOLINE(RegisterAMXFunctions) {
    static const void Callback(u64 param_1) {
        Orig(param_1);
        // TODO: symbol
        // actually registers an array of symbols
        external<void>(0x66cba0, param_1, AMX::custom_symbols.data());
    }
};

HOOK_DEFINE_INLINE(OnAmxCallback) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        AMX::AMX_INSTANCE* instance = reinterpret_cast<AMX::AMX_INSTANCE*>(ctx->X[0]);
        u64* args = reinterpret_cast<u64*>(ctx->X[1]);
        for (auto& callback : AMX::amx_callbacks) {
            callback(instance, args);
        }
    }
};

void install_amx_patch() {
    RegisterAMXFunctions::InstallAtOffset(0x1464ff0 - VER_OFF);
    // amx_Callback
    OnAmxCallback::InstallAtOffset(0x66d04c);
}
