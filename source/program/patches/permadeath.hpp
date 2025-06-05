#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"

void remove_party_member(base_party_t* party, u8 index) {
    std::copy(
        party->members + index + 1,
        party->members + party->count,
        party->members + index
    );
    party->members[party->count] = 0;
    party->count--;
}

HOOK_DEFINE_INLINE(PermaDeath) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.permadeath_enabled) return;
        if (ctx->X[0] == 0) {
            return;
        }
        base_party_t* battle_party = reinterpret_cast<base_party_t*>(ctx->X[0] + 0x8);
        s8 remove_index = -1;
        for (u8 i = 1; i < battle_party->count; i++) {
            if (battle_party->members[i] == nullptr) {
                break;
            }

            if (battle_party->members[i]->hp == 0) {
                remove_index = i;
                break;
            }
        }
        if (remove_index == -1) {
            return;
        }
        remove_party_member(battle_party, remove_index);
    }
};


// the vanilla game expects the size of the party to be the same before & after battle
// if the size decreases, it will attempt to read information about the missing members
// and will nullptr dereference
HOOK_DEFINE_INLINE(FixIterationCount) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.permadeath_enabled) return;
        base_party_t* battle_party = reinterpret_cast<base_party_t*>(ctx->X[23]);
        ctx->W[0] = std::min(static_cast<u8>(ctx->W[0]), battle_party->count);
    }
};

HOOK_DEFINE_INLINE(RestorePartyOrderPatch) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.permadeath_enabled) return;
        base_party_t* battle_party = reinterpret_cast<base_party_t*>(ctx->X[8] + 0x8);
        // if the first member is dead & didn't have a chance to be removed in battle,
        // remove it now
        if (battle_party->count > 1) {
            if (battle_party->members[0] != nullptr) {
                if (battle_party->members[0]->hp == 0) {
                    remove_party_member(battle_party, 0);
                }
            }
        }
        // iteration count issue happens here as well (party count may also be modified)
        ctx->W[21] = std::min(static_cast<u8>(ctx->W[21]), battle_party->count);
        ctx->W[19] = ctx->W[21];
        // fix any bad indices that result from removing members in
        // the middle of the party
        s8 sorted_indicies[6] = {-1, -1, -1, -1, -1, -1};
        for (u32 i = 0; i < ctx->W[21]; i++) {
            if (battle_party->members[i] == 0) {
                break;
            }
            sorted_indicies[battle_party->members[i]->real_party_index] = i;
        }
        u8 fixed_index = 0;
        for (u32 i = 0; i < 6; i++) {
            if (sorted_indicies[i] == -1) {
                continue;
            }
            battle_party->members[sorted_indicies[i]]->real_party_index = fixed_index++;
        }
    }
};

void install_permadeath_patch() {
    // some function called very often in battle that has access to the party
    // TODO: symbol
    PermaDeath::InstallAtOffset(0x7df770);
    RestorePartyOrderPatch::InstallAtOffset(RestorePartyOrder_offset + 0x68);
    // TODO: symbol
    FixIterationCount::InstallAtOffset(0x8de198);
}