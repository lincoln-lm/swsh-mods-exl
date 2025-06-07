enum evolution_type : u16 {
    None = 0,
    LevelUp = 4,
    Trade = 5,
    TradeHeldItem = 6,
    TradeShelmetKarrablast = 7,
    LevelUpHeldItemDay = 19,
    LevelUpWithTeammate = 22,
};

struct evolution_method_t {
    evolution_type method;
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
        if (!save_file.evo_rng.enabled) return;
        PersonalInfo::FetchInfo(species, form);
        u8 base_type_1 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_1);
        u8 base_type_2 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_2);
        u8 base_exp_growth = PersonalInfo::GetField(PersonalInfo::InfoField::EXP_GROWTH);
        if (save_file.evo_rng.each_level) {
            // evolutions get cached so this doesnt actually randomize every time a species needs to evolve.
            // the cache is important because the evolution data gets fetched multiple times in the process
            // of evolving and will break if it does not match.
            // there is probably a sensible way to clear the cache or enable it only for these calls, but for
            // now this behavior is fine.
            auto rng = RngManager::NewRandomGenerator();
            evolution_method_t& method = (*out->methods)[0];
            method.method = evolution_type::LevelUp;
            method.level = 1;
            while (true) {
                auto [evo_species, evo_form] = rng.RandSpeciesAndForm();
                // force changed evo
                if (evo_species == method.species && evo_form == method.form) {
                    continue;
                }
                if (species == method.species && form == method.form) {
                    continue;
                }
                PersonalInfo::FetchInfo(evo_species, evo_form);
                u8 evo_type_1 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_1);
                u8 evo_type_2 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_2);
                u8 evo_exp_growth = PersonalInfo::GetField(PersonalInfo::InfoField::EXP_GROWTH);
                if (evo_exp_growth != base_exp_growth) {
                    continue;
                }
                if (!save_file.evo_rng.match_types
                    || base_type_1 == evo_type_1
                    || base_type_1 == evo_type_2
                    || base_type_2 == evo_type_1
                    || base_type_2 == evo_type_2) {
                    method.species = evo_species;
                    method.form = evo_form;
                    break;
                }
            }
            for (int i = 1; i < 9; i++) {
                evolution_method_t& method = (*out->methods)[i];
                method.method = evolution_type::None;
            }
            return;
        }
        const std::string seed = std::format("evolution_binary_{}_{}", species, form);
        auto rng = RngManager::NewRandomGenerator(seed);
        for (int i = 0; i < 9; i++) {
            evolution_method_t& method = (*out->methods)[i];
            if (method.species == 0) {
                continue;
            }
            // regular trade evos -> level 30
            if (method.method == evolution_type::Trade) {
                method.method = evolution_type::LevelUp;
                method.level = 30;
            // trade held item -> level held item
            } else if (method.method == evolution_type::TradeHeldItem) {
                method.method = evolution_type::LevelUpHeldItemDay;
            // trade with mon -> level with mon
            } else if (method.method == evolution_type::TradeShelmetKarrablast) {
                // karrablast
                if (method.species == 588) {
                    method.argument = 616;
                // shelmet
                } else if (method.species == 616) {
                    method.argument = 588;
                }
            }
            while (true) {
                auto [evo_species, evo_form] = rng.RandSpeciesAndForm();
                // force changed evo
                if (evo_species == method.species && evo_form == method.form) {
                    continue;
                }
                if (evo_species == species && evo_form == form) {
                    continue;
                }
                PersonalInfo::FetchInfo(evo_species, evo_form);
                u8 evo_type_1 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_1);
                u8 evo_type_2 = PersonalInfo::GetField(PersonalInfo::InfoField::TYPE_2);
                u8 evo_exp_growth = PersonalInfo::GetField(PersonalInfo::InfoField::EXP_GROWTH);
                if (evo_exp_growth != base_exp_growth) {
                    continue;
                }
                if (!save_file.evo_rng.match_types
                    || base_type_1 == evo_type_1
                    || base_type_1 == evo_type_2
                    || base_type_2 == evo_type_1
                    || base_type_2 == evo_type_2) {
                    method.species = evo_species;
                    method.form = evo_form;
                    break;
                }
            }
        }
    }
};

void install_evolution_patch() {
    // TODO: symbol LoadEvolutionBinary
    CustomEvolutionBinary::InstallAtOffset(0x786150);
}