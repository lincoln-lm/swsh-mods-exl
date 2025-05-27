struct evolution_method_t {
    u16 method;
    u16 argument;
    u16 species;
    s8 form;
    u8 level;
} PACKED;

struct evolution_set_t {
    u64 unk;
    u32 species;
    u16 form;
    u16 padding;
    evolution_method_t (*methods)[9];
} PACKED;

HOOK_DEFINE_TRAMPOLINE(CustomEvolutionBinary) {
    static void Callback(evolution_set_t* out, u32 species, u32 form) {
        Orig(out, species, form);
        const std::string seed = std::format("evolution_binary_{}_{}", species, form);
        auto rng = RngManager::NewRandomGenerator(seed);
        for (int i = 0; i < 9; i++) {
            evolution_method_t& method = (*out->methods)[i];
            // maintain vanilla method but swap species
            auto [species, form] = rng.RandSpeciesAndForm();
            method.species = species;
            method.form = form;
        }
    }
};

void install_evolution_patch() {
    // TODO: symbol LoadEvolutionBinary
    CustomEvolutionBinary::InstallAtOffset(0x786150 - VER_OFF);
}