#include "std.inc"

// TODO: fix name lol
// WaitUntilMessageWindowIsLoaded

native AddNewSeedDigit(value);
native ResetSeed();
native ToggleSetting(index);

main()
{
    switch (g_mode) {
        case "settings_dialog":
        {
            new page = 0;
            while (true) {
                ShowMessageWindow(0, GetFnvHash64("ask_settings"), 100, 3, 1, 0, 0);
                switch (page) {
                    case 0:
                    {
                        WaitUntilMessageWindowIsLoaded(0, GetFnvHash64("option_confirm_settings"), 1);
                        WaitUntilMessageWindowIsLoaded(1, GetFnvHash64("option_show_seed"), 1);
                        WaitUntilMessageWindowIsLoaded(2, GetFnvHash64("option_new_seed"), 1);
                        WaitUntilMessageWindowIsLoaded(3, GetFnvHash64("option_evolution"), 1);
                        WaitUntilMessageWindowIsLoaded(4, GetFnvHash64("option_learnset"), 1);
                        WaitUntilMessageWindowIsLoaded(5, GetFnvHash64("option_items"), 1);
                        WaitUntilMessageWindowIsLoaded(6, GetFnvHash64("option_next"), 1);
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: {
                                break;
                            }
                            case 1:
                            {
                                ShowMessageWindow(0, GetFnvHash64("show_seed"), 100, 3, 1, 0, 0);
                                ABKeyWait_();
                            }
                            case 2:
                            {
                                ResetSeed();
                                new seed_page = 0;
                                for (new i = 0; i < 20; i++) {
                                    while (true) {
                                        ShowMessageWindow(0, GetFnvHash64("show_seed"), 100, 3, 1, 0, 0);
                                        switch (seed_page) {
                                            case 0:
                                            {
                                                WaitUntilMessageWindowIsLoaded(0, GetFnvHash64("0"), 1);
                                                WaitUntilMessageWindowIsLoaded(1, GetFnvHash64("1"), 1);
                                                WaitUntilMessageWindowIsLoaded(2, GetFnvHash64("2"), 1);
                                                WaitUntilMessageWindowIsLoaded(3, GetFnvHash64("3"), 1);
                                                WaitUntilMessageWindowIsLoaded(4, GetFnvHash64("4"), 1);
                                                WaitUntilMessageWindowIsLoaded(5, GetFnvHash64("5"), 1);
                                                WaitUntilMessageWindowIsLoaded(6, GetFnvHash64("option_next"), 1);
                                                new seed_response = RequestListInput(1, 0, 1, 0);
                                                CloseMessageWindow();
                                                if (seed_response == 6) {
                                                    seed_page++;
                                                } else {
                                                    AddNewSeedDigit(seed_response);
                                                    break;
                                                }
                                            }
                                            case 1:
                                            {
                                                WaitUntilMessageWindowIsLoaded(0, GetFnvHash64("option_back"), 1);
                                                WaitUntilMessageWindowIsLoaded(1, GetFnvHash64("6"), 1);
                                                WaitUntilMessageWindowIsLoaded(2, GetFnvHash64("7"), 1);
                                                WaitUntilMessageWindowIsLoaded(3, GetFnvHash64("8"), 1);
                                                WaitUntilMessageWindowIsLoaded(4, GetFnvHash64("9"), 1);
                                                new seed_response = RequestListInput(1, 0, 1, 0);
                                                CloseMessageWindow();
                                                if (seed_response == 0) {
                                                    seed_page--;
                                                } else {
                                                    AddNewSeedDigit(seed_response + 5);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            case 3: ToggleSetting(0);
                            case 4: ToggleSetting(1);
                            case 5: ToggleSetting(2);
                            case 6:
                            {
                                page++;
                            }
                        }
                    }
                    case 1:
                    {
                        WaitUntilMessageWindowIsLoaded(0, GetFnvHash64("option_back"), 1);
                        WaitUntilMessageWindowIsLoaded(1, GetFnvHash64("option_personal"), 1);
                        WaitUntilMessageWindowIsLoaded(2, GetFnvHash64("option_trainer_poke"), 1);
                        WaitUntilMessageWindowIsLoaded(3, GetFnvHash64("option_wild_pokemon"), 1);
                        WaitUntilMessageWindowIsLoaded(4, GetFnvHash64("option_models"), 1);
                        WaitUntilMessageWindowIsLoaded(5, GetFnvHash64("option_permadeath"), 1);
                        WaitUntilMessageWindowIsLoaded(6, GetFnvHash64("option_next"), 1);
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: 
                            {
                                page--;
                            }
                            case 1: ToggleSetting(3);
                            case 2: ToggleSetting(4);
                            case 3: ToggleSetting(5);
                            case 4: ToggleSetting(6);
                            case 5: ToggleSetting(7);
                            case 6:
                            {
                                page++;
                            }
                        }
                    }
                    case 2:
                    {
                        WaitUntilMessageWindowIsLoaded(0, GetFnvHash64("option_back"), 1);
                        WaitUntilMessageWindowIsLoaded(1, GetFnvHash64("option_route_restriction"), 1);
                        WaitUntilMessageWindowIsLoaded(2, GetFnvHash64("option_level_cap"), 1);
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: 
                            {
                                page--;
                            }
                            case 1: ToggleSetting(8);
                            case 2: ToggleSetting(9);
                        }
                    }
                }
            }
        }
        default:
            CommandNOP()
    }
}