#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "rng_manager.hpp"

HOOK_DEFINE_INLINE(RandomPokeData) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (!save_file.trainer_rng.enabled) return;
        // TODO: GFFile struct
        const std::span<const char> file_path(
            *reinterpret_cast<const char**>(ctx->X[20] + 0x70),
            *reinterpret_cast<const size_t*>(ctx->X[20] + 0x78)
        );
        // gamefreak is dumb and always memcpys 192 bytes from this file buffer
        // if it memcpyd properly, the buffer size would be in x2
        u64 file_buffer_size = *reinterpret_cast<u64*>(ctx->X[20] + 0x1a0);
        int team_size = file_buffer_size / sizeof(trainer_poke_data_t);
        auto rng = RngManager::NewRandomGenerator(file_path);
        auto trainer_team = reinterpret_cast<trainer_poke_data_t*>(ctx->X[1]);
        for (int i = 0; i < team_size; i++) {
            if (trainer_team[i].level == 0 || trainer_team[i].level > 100 || trainer_team[i].species == 0) {
                continue;
            }
            trainer_team[i].level *= 1.5;
            auto [
                species,
                form
            ] = rng.RandSpeciesAndForm();
            trainer_team[i].species = species;
            trainer_team[i].form = form;
            // TODO: gender ratios
            trainer_team[i].gender = rng.RandMax(2);
            trainer_team[i].ability = rng.RandMax(3);
            trainer_team[i].held_item = rng.RandHeldItem();
            rng.RandMoves(std::span(trainer_team[i].moves));

            // TODO: randomize nature/ivs/evs?
            trainer_team[i].shiny = rng.RandChance(4096);
        }
    }
};

void install_trainer_poke_patch() {
    RandomPokeData::InstallAtOffset(LoadTrainerPokeData_offset + 0xF8);
}