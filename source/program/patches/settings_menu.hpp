#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include <map>

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
        case 0:
            save_file.evo_rng.enabled ^= 1;
            save_file.evo_rng.match_types &= save_file.evo_rng.enabled;
            save_file.evo_rng.each_level &= save_file.evo_rng.enabled;
            break;
        case 1:
            save_file.evo_rng.match_types ^= 1;
            save_file.evo_rng.match_types &= save_file.evo_rng.enabled;
            break;
        case 2:
            save_file.evo_rng.each_level ^= 1;
            save_file.evo_rng.each_level &= save_file.evo_rng.enabled;
            break;
        case 3: save_file.learnset_rng.enabled ^= 1; break;
        case 4: save_file.item_rng.enabled ^= 1; break;
        case 5: save_file.personal_rng.enabled ^= 1; break;
        case 6:
            save_file.trainer_rng.enabled ^= 1;
            break;
        case 7:
            save_file.trainer_rng.level_boost ^= 1;
            break;
        case 8:
            save_file.wild_rng.enabled ^= 1;
            save_file.wild_rng.live &= save_file.wild_rng.enabled;
            break;
        case 9:
            save_file.wild_rng.live ^= 1;
            save_file.wild_rng.live &= save_file.wild_rng.enabled;
            break;
        case 10:
            save_file.wild_rng.level_boost ^= 1;
            break;
        case 11: save_file.scripted_rng.enabled ^= 1; break;
        case 12: save_file.gift_rng.enabled ^= 1; break;
        case 13:
            save_file.model_rng.enabled ^= 1;
            save_file.model_rng.hide_starters &= save_file.model_rng.enabled;
            break;
        case 14:
            save_file.model_rng.hide_starters ^= 1;
            save_file.model_rng.hide_starters &= save_file.model_rng.enabled;
            break;
        case 15: save_file.permadeath.enabled ^= 1; break;
        case 16: save_file.route_restriction.enabled ^= 1; break;
        case 17: save_file.level_cap_boost.enabled ^= 1; break;
        case 18:
            save_file.filter_shop_items.enabled ^= 1;
            break;
        }
    return 1;
}

HOOK_DEFINE_TRAMPOLINE(SettingsMenuTrigger) {
    static const u64 Callback(u64 param_1) {
        if (save_file.settings_menu_state == 0) {
            AMX::call_pawn_script("settings_dialog");
        } else if (save_file.settings_menu_state == 1) {
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
        } else if (save_file.settings_menu_state >= 2) {
            return Orig(param_1);
        }
        save_file.settings_menu_state++;
        return 1;
    }
};

struct fake_gf_string_t {
    u64 unk_0[0x58/8];
    const char16_t* string;
};

static u64 last_hash = 0;

#define BASIC_MESSAGE(identifier, message) \
    {getConstFNV1aHashedString(identifier).hash, []() { return u ## message; }}
#define TOGGLE_MESSAGE(identifier, message, flag) \
    {getConstFNV1aHashedString(identifier).hash, []() { return flag ? u ## message u" (on)" : u ## message u" (off)"; }}
static const std::map<u64, std::function<const char16_t*()>> custom_messages = {
    BASIC_MESSAGE("ask_settings", "Configure challenge settings."),
    BASIC_MESSAGE("option_confirm_settings", "Confirm settings"),
    BASIC_MESSAGE("option_confirm", "Confirm"),
    BASIC_MESSAGE("option_next", "Next"),
    BASIC_MESSAGE("option_back", "Back"),
    BASIC_MESSAGE("option_new_seed", "Enter custom seed"),
    BASIC_MESSAGE("option_show_seed", "Show current seed"),
    BASIC_MESSAGE("0", "0"),
    BASIC_MESSAGE("1", "1"),
    BASIC_MESSAGE("2", "2"),
    BASIC_MESSAGE("3", "3"),
    BASIC_MESSAGE("4", "4"),
    BASIC_MESSAGE("5", "5"),
    BASIC_MESSAGE("6", "6"),
    BASIC_MESSAGE("7", "7"),
    BASIC_MESSAGE("8", "8"),
    BASIC_MESSAGE("9", "9"),
    TOGGLE_MESSAGE("option_evo_rng", "Toggle evolution randomization", save_file.evo_rng.enabled),
    TOGGLE_MESSAGE("option_evo_match_types", "Toggle evolutions match types", save_file.evo_rng.match_types),
    TOGGLE_MESSAGE("option_evo_each_level", "Toggle evolve each level", save_file.evo_rng.each_level),
    TOGGLE_MESSAGE("option_learnset_rng", "Toggle learnset randomization", save_file.learnset_rng.enabled),
    TOGGLE_MESSAGE("option_item_rng", "Toggle item randomization", save_file.item_rng.enabled),
    TOGGLE_MESSAGE("option_personal_rng", "Toggle personal info randomization", save_file.personal_rng.enabled),
    TOGGLE_MESSAGE("option_trainer_rng", "Toggle trainer randomization", save_file.trainer_rng.enabled),
    TOGGLE_MESSAGE("option_trainer_level_boost", "Toggle 1.5x trainer level", save_file.trainer_rng.level_boost),
    TOGGLE_MESSAGE("option_wild_rng", "Toggle wild randomization", save_file.wild_rng.enabled),
    TOGGLE_MESSAGE("option_wild_live_rng", "Toggle live wild randomization", save_file.wild_rng.live),
    TOGGLE_MESSAGE("option_wild_level_boost", "Toggle 1.5x wild level", save_file.wild_rng.level_boost),
    TOGGLE_MESSAGE("option_scripted_rng", "Toggle scripted encounter randomization", save_file.scripted_rng.enabled),
    TOGGLE_MESSAGE("option_gift_rng", "Toggle gift randomization", save_file.gift_rng.enabled),
    TOGGLE_MESSAGE("option_model_rng", "Toggle Pokémon model randomization", save_file.model_rng.enabled),
    TOGGLE_MESSAGE("option_model_hide_starters", "Hide starter randomization", save_file.model_rng.hide_starters),
    TOGGLE_MESSAGE("option_permadeath", "Toggle nuzlocke permadeath", save_file.permadeath.enabled),
    TOGGLE_MESSAGE("option_route_restriction", "Toggle nuzlocke route restriction", save_file.route_restriction.enabled),
    TOGGLE_MESSAGE("option_level_cap_boost", "Toggle 1.5x level cap", save_file.level_cap_boost.enabled),
    TOGGLE_MESSAGE("option_filter_shop_items", "Toggle shop item filter", save_file.filter_shop_items.enabled)
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
                for (u64 i = 0; i < decimal_repr.length(); i++) {
                    replacement_buffer[end_ptr++] = decimal_repr[i];
                }
                replacement_buffer[end_ptr++] = u'\000';
            }
            replacement.string = replacement_buffer;
            src = &replacement;
        } else {
            auto result = custom_messages.find(last_hash);
            if (result != custom_messages.end()) {
                replacement.string = result->second();
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
    MsgStringReplace::InstallAtOffset(0x67d4d0);
    MsgHashStore::InstallAtOffset(0x67eb10);
    ValidateMessages::InstallAtOffset(0x13bca40 - VER_OFF);
    AMX::add_new_symbol("AddNewSeedDigit", AddNewSeedDigit);
    AMX::add_new_symbol("ResetSeed", ResetSeed);
    AMX::add_new_symbol("ToggleSetting", ToggleSetting);
}
