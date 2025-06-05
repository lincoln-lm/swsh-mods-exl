#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include <map>

static u8 settings_menu_state = 0;

u64 AddNewSeedDigit(void* amx, u64* params) {
    save_file.rng_seed *= 10;
    save_file.rng_seed += params[1];
    save_file.rng_seed_set = true;
    return 1;
}

u64 ResetSeed(void* amx, u64* params) {
    save_file.rng_seed = 0;
    save_file.rng_seed_set = false;
    return 1;
}

u64 ToggleSetting(void* amx, u64* params) {
    switch (params[1]) {
        case 0: save_file.evolution_randomization_enabled ^= 1; break;
        case 1: save_file.learnset_randomization_enabled ^= 1; break;
        case 2: save_file.item_randomization_enabled ^= 1; break;
        case 3: save_file.personal_randomization_enabled ^= 1; break;
        case 4: save_file.trainer_poke_randomization_enabled ^= 1; break;
        case 5: save_file.wild_pokemon_randomization_enabled ^= 1; break;
        case 6: save_file.model_randomization_enabled ^= 1; break;
        case 7: save_file.permadeath_enabled ^= 1; break;
        case 8: save_file.route_restriction_enabled ^= 1; break;
        case 9: save_file.level_cap_enabled ^= 1; break;
    }
    return 1;
}

HOOK_DEFINE_TRAMPOLINE(SettingsMenuTrigger) {
    static const u64 Callback(u64 param_1) {
        if (settings_menu_state == 0) {
            AMX::call_pawn_script("settings_dialog");
        } else if (settings_menu_state == 1) {
            // TODO: doc
            u64 scene_transition_data[31] = {
                0x4,
                0x0,
                getConstFNV1aHashedString("a_t0101_i0101").hash,
                getConstFNV1aHashedString("").hash,
                0x0,
                0x1,
                // position data, floats
                0x4000000044902000,
                0x448de000,
                0x3f80000000000000,
                0x3380000000000000,
                0x5f31303130745f61,
                0x3130313069,
                0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                getConstFNV1aHashedString("").hash,
                0x10100,
                0xffffffff,
                getConstFNV1aHashedString("").hash,
                0x0, 0x0, 0x0,
                getConstFNV1aHashedString("").hash,
                main_offset(0x1c3857a),
                0x0, 0x0, 0x0
            };
            // hacky: pad to >128 to trigger return storage
            struct unk_holder_t {
                u64 unk_0;
                u64 unk_1;
                u64 unk_2;
            };
            // TODO: symbol
            // triggers scene transitions
            // in this instance it is used to reload a_t0101_i0101
            // (the player's house)
            external<unk_holder_t>(0xdf2cd0 - VER_OFF, scene_transition_data);
        } else if (settings_menu_state >= 2) {
            return Orig(param_1);
        }
        settings_menu_state++;
        return 1;
    }
};

struct fake_gf_string_t {
    u64 unk_0[0x58/8];
    const char16_t* string;
};

static u64 last_hash = 0;

static const std::map<u64, const char16_t*> custom_messages = {
    {getConstFNV1aHashedString("ask_settings").hash, u"Configure challenge settings."},
    {getConstFNV1aHashedString("option_confirm_settings").hash, u"Confirm settings"},
    {getConstFNV1aHashedString("option_confirm").hash, u"Confirm"},
    {getConstFNV1aHashedString("option_next").hash, u"Next"},
    {getConstFNV1aHashedString("option_back").hash, u"Back"},
    {getConstFNV1aHashedString("option_new_seed").hash, u"Enter custom seed"},
    {getConstFNV1aHashedString("option_show_seed").hash, u"Show current seed"},
    {getConstFNV1aHashedString("0").hash, u"0"},
    {getConstFNV1aHashedString("1").hash, u"1"},
    {getConstFNV1aHashedString("2").hash, u"2"},
    {getConstFNV1aHashedString("3").hash, u"3"},
    {getConstFNV1aHashedString("4").hash, u"4"},
    {getConstFNV1aHashedString("5").hash, u"5"},
    {getConstFNV1aHashedString("6").hash, u"6"},
    {getConstFNV1aHashedString("7").hash, u"7"},
    {getConstFNV1aHashedString("8").hash, u"8"},
    {getConstFNV1aHashedString("9").hash, u"9"},
};

HOOK_DEFINE_TRAMPOLINE(MsgStringReplace) {
    static u16 Callback(fake_gf_string_t* dst, fake_gf_string_t* src) {
        fake_gf_string_t replacement;
        char16_t replacement_buffer[0x200];
        // this is very cursed
        if (last_hash == getConstFNV1aHashedString("show_seed").hash) {
            std::char_traits<char16_t>::copy(replacement_buffer, u"Current seed: ", 14);
            if (save_file.rng_seed_set) {
                auto end_ptr = std::char_traits<char16_t>::length(replacement_buffer);
                // ceil(64/log2(10))
                auto decimal_repr = std::format("{:020d}", save_file.rng_seed);
                for (int i = 0; i < decimal_repr.length(); i++) {
                    replacement_buffer[end_ptr++] = decimal_repr[i];
                }
                replacement_buffer[end_ptr++] = u'\000';
            }
            replacement.string = replacement_buffer;
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_evolution").hash) {
            if (save_file.evolution_randomization_enabled) {
                replacement.string = u"Toggle evolution randomization (on)";
            } else {
                replacement.string = u"Toggle evolution randomization (off)";
            }
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_learnset").hash) {
            if (save_file.learnset_randomization_enabled) {
                replacement.string = u"Toggle learnset randomization (on)";
            } else {
                replacement.string = u"Toggle learnset randomization (off)";
            }
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_items").hash) {
            if (save_file.item_randomization_enabled) {
                replacement.string = u"Toggle item randomization (on)";
            } else {
                replacement.string = u"Toggle item randomization (off)";
            }
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_personal").hash) {
            if (save_file.personal_randomization_enabled) {
                replacement.string = u"Toggle personal info randomization (on)";
            } else {
                replacement.string = u"Toggle personal info randomization (off)";
            }
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_trainer_poke").hash) {
            if (save_file.trainer_poke_randomization_enabled) {
                replacement.string = u"Toggle trainer randomization (on)";
            } else {
                replacement.string = u"Toggle trainer randomization (off)";
            }
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_wild_pokemon").hash) {
            if (save_file.wild_pokemon_randomization_enabled) {
                replacement.string = u"Toggle wild randomization (on)";
            } else {
                replacement.string = u"Toggle wild randomization (off)";
            }
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_models").hash) {
            if (save_file.model_randomization_enabled) {
                replacement.string = u"Toggle pokemon model randomization (on)";
            } else {
                replacement.string = u"Toggle pokemon model randomization (off)";
            }
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_permadeath").hash) {
            if (save_file.permadeath_enabled) {
                replacement.string = u"Toggle nuzlocke permadeath (on)";
            } else {
                replacement.string = u"Toggle nuzlocke permadeath (off)";
            }
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_route_restriction").hash) {
            if (save_file.route_restriction_enabled) {
                replacement.string = u"Toggle nuzlocke route restriction (on)";
            } else {
                replacement.string = u"Toggle nuzlocke route restriction (off)";
            }
            src = &replacement;
        } else if (last_hash == getConstFNV1aHashedString("option_level_cap").hash) {
            if (save_file.level_cap_enabled) {
                replacement.string = u"Toggle level cap boost (on)";
            } else {
                replacement.string = u"Toggle level cap boost (off)";
            }
            src = &replacement;
        } else {
            auto result = custom_messages.find(last_hash);
            if (result != custom_messages.end()) {
                replacement.string = result->second;
                src = &replacement;
            }
        }
        last_hash = 0;
        return Orig(dst, src);
    }
};

HOOK_DEFINE_INLINE(MsgHashStore) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        last_hash = *reinterpret_cast<u64*>(ctx->X[1]);
    }
};

HOOK_DEFINE_TRAMPOLINE(ValidateMessages) {
    static bool Callback(u64 param_1) {
        return 1;
    }
};

void install_settings_menu_patch() {
    SettingsMenuTrigger::InstallAtOffset(0x14c8ac0 - VER_OFF);
    MsgStringReplace::InstallAtOffset(0x67d4d0 - VER_OFF);
    MsgHashStore::InstallAtOffset(0x67eb10 - VER_OFF);
    ValidateMessages::InstallAtOffset(0x13bca40 - VER_OFF);
    AMX::add_new_symbol("AddNewSeedDigit", AddNewSeedDigit);
    AMX::add_new_symbol("ResetSeed", ResetSeed);
    AMX::add_new_symbol("ToggleSetting", ToggleSetting);
}