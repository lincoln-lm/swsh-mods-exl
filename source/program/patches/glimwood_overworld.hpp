#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"
#include <unordered_set>

// TODO: crack hashes?
const std::unordered_set<u64> glimwood_spawners = {
    0x70478112111c5ac1,
    0xc0ff974997204bbe,
    0xc0ff984997204d71,
    0xc0ff954997204858,
    0xc0ff964997204a0b,
    0xc0ff9b499720528a,
    0xc0ff9c499720543d,
    0xc0ff994997204f24,
    0xc0ff9a49972050d7,
    0xc0ff8f4997203e26,
    0xc0ff904997203fd9,
    0x129d060bcbe34573,
    0x129d050bcbe343c0,
    0x129d080bcbe348d9,
    0x129d070bcbe34726,
    0x129d0a0bcbe34c3f,
    0x129d090bcbe34a8c,
    0x129d0c0bcbe34fa5,
    0x129d0b0bcbe34df2,
};

HOOK_DEFINE_INLINE(PatchMaximumSpawns) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.glimwood_overworld.active) {
            auto spawner = reinterpret_cast<Field::EncountSpawner*>(ctx->X[19]);
            if (glimwood_spawners.find(spawner->unique_hash) != glimwood_spawners.end()) {
                spawner->maximum_symbol_encounts = global_config.glimwood_overworld.maximum_spawns;
            }
        }
    }
};

void install_glimwood_overworld_patch() {
    PatchMaximumSpawns::InstallAtOffset(Field::EncountSpawner::EncountSpawner_offset + 0x150);
}