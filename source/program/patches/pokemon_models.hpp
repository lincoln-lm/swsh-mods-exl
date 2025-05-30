#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "rng_manager.hpp"

static bool is_encount_object = false;

static void replace_species_form(u64 hash, s32* species_ptr, s16* form_ptr) {
    u64 seed;
    // use gift's add_pokemon hash for the seed
    // TODO: toxel, type: null, kanto starters, kubfu, cosmog, poipole
    // TODO: scripted encounters
    // sobble
    if (hash == getConstFNV1aHashedString("z_t0101_MIZU").hash) {
        seed = 0x289d0e4aa0fd660d;
    // scorbunny
    } else if (hash == getConstFNV1aHashedString("z_t0101_HONO").hash) {
        seed = 0xe611a220de507f60;
    // grookey
    } else if (hash == getConstFNV1aHashedString("z_t0101_KUSA").hash) {
        seed = 0xd0f69218d6d84126;
    // fog zamazenta 
    } else if (hash == getConstFNV1aHashedString("z_d0101_PM_889_OOKAMIT").hash) {
        seed = 0x6f0b36ae4a5df4a9;
        // fog zacian z_d0101_PM_888_OOKAMIK
    // toxel gift
    } else if (hash == getConstFNV1aHashedString("z_r0501_i0101_PM_848_PUNKII1").hash) {
        seed = 0x534abd7f00ee7e61;
    } else {
        // it would be neat to randomize all models based on their hash
        // but there is something else limiting how many unique pokemon
        // npcs can exist.
        // randomizing based on species/form maps 1:1 from the game
        // which should guarantee to dodge this limit.
        seed = (*species_ptr) | (*form_ptr) << 16;
    }
    // randomize the same as gifts.hpp
    auto rng = RngManager::NewRandomGenerator(seed);
    auto [species, form] = rng.RandSpeciesAndForm();
    *species_ptr = species;
    *form_ptr = form;
}

HOOK_DEFINE_INLINE(RandomizePokemonModelsOnLoad) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto fb = reinterpret_cast<FlatbufferObjects::PokemonModel*>(ctx->X[27]);
        u64 sp = ctx->X[1] - 0x100;
        u64 hash = fb->inner()->inner()->field_object()->unique_hash();
        s32* species_ptr = reinterpret_cast<s32*>(sp + 0xE0);
        s16* form_ptr = reinterpret_cast<s16*>(sp + 0xE4);
        replace_species_form(hash, species_ptr, form_ptr);
        ctx->W[0] = *species_ptr;
        ctx->W[8] = *form_ptr;
    }
};

HOOK_DEFINE_INLINE(RandomizePokemonModels) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        // TODO: this feels hacky
        // is there a more sensible way to detect if the ctor is called
        // within the EncountObject ctor
        if (is_encount_object) {
            is_encount_object = false;
            return;
        }
        u64 pokemon_model = ctx->X[19];
        u64 hash = reinterpret_cast<Field::FieldObject*>(pokemon_model)->unique_hash;
        s32* species_ptr = reinterpret_cast<s32*>(pokemon_model + 0x5cc);
        s16* form_ptr = reinterpret_cast<s16*>(pokemon_model + 0x5d0);
        replace_species_form(hash, species_ptr, form_ptr);
    }
};

HOOK_DEFINE_INLINE(LogEncountObject) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        is_encount_object = true;
    }
};

HOOK_DEFINE_TRAMPOLINE(PatchNullPtrDeref) {
    static void Callback(u64 param_1, u64** param_2, u32 param_3) {
        if (param_2[1] == nullptr) {
            return;
        }
        Orig(param_1, param_2, param_3);
    }
};

void install_pokemon_models_patch() {
    // TODO: symbol
    // something that reads the fbs to do some preloading before the map loads
    // if too many unique models that arent preloaded attempt to spawn
    // the game will hang when loading
    RandomizePokemonModelsOnLoad::InstallAtOffset(0xd0ca68 - VER_OFF);
    // TODO: symbol
    // PokemonModel::PokemonModel + ???
    RandomizePokemonModels::InstallAtOffset(0xda88c4 - VER_OFF);
    // TODO: symbol
    // EncountObject::EncountObject + ???
    LogEncountObject::InstallAtOffset(0xd5dabc - VER_OFF);
    // TODO: symbol
    // some function that causes a nullptr deref in some circumstances
    // (specifically triggered when replacing some models with zekrom)
    // stubbing it when the ptr is null seems to work fine
    PatchNullPtrDeref::InstallAtOffset(0xd1be90 - VER_OFF);
}

