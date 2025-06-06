#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "rng_manager.hpp"

struct encounter_event_t {
    u64 hash;
    u32 species;
    u16 form;
    u16 padding_0;
    u32 unk_0;
    u32 level;
    u32 shiny_lock;
    u32 gender;
    u32 nature;
    u32 ability;
    u16 held_item;
    u8 padding_1[6];
    u64 background_far_type_id;
    u64 background_near_type_id;
    u32 encounter_scenario;
    u32 moves[4];
    u8 dynamax_level;
    u8 unk_1;
    bool can_gigantamax;
    u8 ivs[6];
    u8 evs[6];
    u8 unk_2;
} PACKED;
static_assert(sizeof(encounter_event_t) == 0x64);

HOOK_DEFINE_INLINE(RandomizeEventEncounters) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.scripted_rng.enabled) return;
        auto encount = reinterpret_cast<encounter_event_t*>(ctx->X[0]);
        auto rng = RngManager::NewRandomGenerator(encount->hash);
        auto [species, form] = rng.RandSpeciesAndForm();
        std::array<s16, 4> moves;
        rng.RandMoves(std::span(moves));
        encount->species = species;
        encount->form = form;
        encount->shiny_lock = 0;
        encount->nature = 25;
        encount->ability = rng.RandMax(3);
        encount->held_item = rng.RandHeldItem();
        std::copy(
            moves.begin(),
            moves.end(),
            encount->moves
        );
    }
};

void install_event_encounters_patch() {
    // TODO: symbol
    // UnpackEncounterEventArchive + ???
    RandomizeEventEncounters::InstallAtOffset(0xe4c1b8 - VER_OFF);
}

