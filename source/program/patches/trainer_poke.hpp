#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "rng_manager.hpp"

HOOK_DEFINE_INLINE(RandomPokeData) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        // TODO: GFFile struct
        const std::span<const char> file_path(
            *reinterpret_cast<const char**>(ctx->X[20] + 0x70),
            *reinterpret_cast<const size_t*>(ctx->X[20] + 0x78)
        );
        auto rng = RngManager::NewRandomGenerator(file_path);
        auto trainer_team = reinterpret_cast<trainer_poke_data_t*>(ctx->X[1]);
        for (int i = 0; i < 6; i++) {
            if (trainer_team[i].level == 0 || trainer_team[i].level > 100) {
                break;
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