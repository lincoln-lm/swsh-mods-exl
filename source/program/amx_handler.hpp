#pragma once
#include "lib.hpp"
#include "util.hpp"

static const char16_t* curr_value = nullptr;
static const char16_t* prev_value = nullptr;
static const char16_t** name_loc = nullptr;
static bool should_replace = false;

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

    // TODO: replace this with msg_handler
    HOOK_DEFINE_INLINE(WordSetRegister_Custom) {
        static void Callback(exl::hook::nx64::InlineCtx* ctx) {
            prev_value = nullptr;
            name_loc = nullptr;
            if (curr_value != nullptr && should_replace) {
                name_loc = reinterpret_cast<const char16_t**>(ctx->X[1] + 0x58);
                prev_value = *name_loc;
                *name_loc = curr_value;
            }
        };
    };
    HOOK_DEFINE_INLINE(ExitWordSetRegister_Custom) {
        static void Callback(exl::hook::nx64::InlineCtx* ctx) {
            if (name_loc != nullptr) {
                *name_loc = prev_value;
            }
            should_replace = false;
        };
    };
    HOOK_DEFINE_INLINE(PG_WordSetRegisterType) {
        static void Callback(exl::hook::nx64::InlineCtx* ctx) {
            if (ctx->W[8] == 0x20) {
                // player name
                ctx->W[8] = 1;
                should_replace = true;
            }
        };
    };

    void set_custom_register_value(const char16_t* value) {
        curr_value = value;
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

    void show_custom_message(const char16_t* message) {
        set_custom_register_value(message);
        call_pawn_script("custom_message");
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
    AMX::PG_WordSetRegisterType::InstallAtOffset(AMX::PG_WordSetRegister_offset + 0xf8);
    AMX::WordSetRegister_Custom::InstallAtOffset(AMX::WordSetRegister_PlayerName_offset + 0xa4);
    AMX::ExitWordSetRegister_Custom::InstallAtOffset(AMX::WordSetRegister_PlayerName_offset + 0xa8);
    RegisterAMXFunctions::InstallAtOffset(0x1464ff0 - VER_OFF);
}
