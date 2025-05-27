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
        PersonalInfo::FetchInfo(species, form);
        u8 base_type_1 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_1);
        u8 base_type_2 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_2);
        u8 base_exp_growth = PersonalInfo::GetField(PersonalInfo::InfoField::EXP_GROWTH);
        for (int i = 0; i < 9; i++) {
            evolution_method_t& method = (*out->methods)[i];
            if (method.species == 0) {
                continue;
            }
            while (true) {
                auto [species, form] = rng.RandSpeciesAndForm();
                // force changed evo
                if (species == method.species && form == method.form) {
                    continue;
                }
                PersonalInfo::FetchInfo(species, form);
                u8 evo_type_1 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_1);
                u8 evo_type_2 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_2);
                u8 evo_exp_growth = PersonalInfo::GetField(PersonalInfo::InfoField::EXP_GROWTH);
                if (evo_exp_growth != base_exp_growth) {
                    continue;
                }
                if (base_type_1 == evo_type_1 || base_type_1 == evo_type_2 ||
                    base_type_2 == evo_type_1 || base_type_2 == evo_type_2) {
                    method.species = species;
                    method.form = form;
                    break;
                }
            }
        }
    }
};

void install_evolution_patch() {
    // TODO: symbol LoadEvolutionBinary
    CustomEvolutionBinary::InstallAtOffset(0x786150 - VER_OFF);
}