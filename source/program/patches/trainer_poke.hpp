#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"

HOOK_DEFINE_INLINE(RandomPokeData) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trainer_team = reinterpret_cast<trainer_poke_data_t*>(ctx->X[1]);
        for (int i = 0; i < 6; i++) {
            if (trainer_team[i].level == 0 || trainer_team[i].level > 100) {
                break;
            }
            trainer_team[i].level *= 1.5;
            auto [species, form] = random_species_and_form();
            // TODO: gender ratios
            trainer_team[i].gender = exl::util::GetRandomU64() % 2;
            trainer_team[i].ability = exl::util::GetRandomU64() % 3;
            trainer_team[i].species = species;
            trainer_team[i].form = form;
            // TODO: randomize held item?
            for (int j = 0; j < 4; j++) {
                // TODO: unique moves
                trainer_team[i].moves[j] = random_valid_move_id();
            }
            // TODO: randomize nature/ivs/evs?
            trainer_team[i].shiny = (exl::util::GetRandomU64() % 4096) == 0;
            
        }
    }
};

void install_trainer_poke_patch() {
    RandomPokeData::InstallAtOffset(LoadTrainerPokeData_offset + 0xF8);
}