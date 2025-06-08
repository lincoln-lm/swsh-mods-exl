#pragma once
#include "lib.hpp"
#include "util.hpp"


namespace AMX {
    struct Symbol {
        const char* name;
        u64 (*function)(void*, u64*);
    };
    std::vector<Symbol> custom_symbols = {
        {nullptr, nullptr}
    };
    void add_new_symbol(const char* name, u64 (*function)(void*, u64*)) {
        AMX::custom_symbols.emplace(AMX::custom_symbols.end() - 1, name, function);
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

void install_amx_patch() {
    RegisterAMXFunctions::InstallAtOffset(0x1464ff0 - VER_OFF);
}
