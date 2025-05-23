#pragma once
#include "lib.hpp"
#include "util.hpp"

namespace AMX {
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

void install_amx_patch() {
    // ...
}
