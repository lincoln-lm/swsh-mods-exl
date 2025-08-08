#include "std.inc"


native AddNewSeedDigit(value);
native ResetSeed();
native ToggleSetting(index);

AddListItem(index, identifier[]) {
    // TODO: fix name lol
    // WaitUntilMessageWindowIsLoaded
    WaitUntilMessageWindowIsLoaded(index, GetFnvHash64(identifier), 1);
}

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
                        AddListItem(0, "option_confirm_settings");
                        AddListItem(1, "option_show_seed");
                        AddListItem(2, "option_new_seed");
                        AddListItem(3, "option_evo_rng");
                        AddListItem(4, "option_evo_match_types");
                        AddListItem(5, "option_evo_each_level");
                        AddListItem(6, "option_next");
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
                                                AddListItem(0, "0");
                                                AddListItem(1, "1");
                                                AddListItem(2, "2");
                                                AddListItem(3, "3");
                                                AddListItem(4, "4");
                                                AddListItem(5, "5");
                                                AddListItem(6, "option_next");
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
                                                AddListItem(0, "option_back");
                                                AddListItem(1, "6");
                                                AddListItem(2, "7");
                                                AddListItem(3, "8");
                                                AddListItem(4, "9");
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
                        AddListItem(0, "option_back");
                        AddListItem(1, "option_learnset_rng");
                        AddListItem(2, "option_item_rng");
                        AddListItem(3, "option_gift_tms_rng");
                        AddListItem(4, "option_personal_rng");
                        AddListItem(5, "option_trainer_rng");
                        AddListItem(6, "option_next");
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
                        AddListItem(0, "option_back");
                        AddListItem(1, "option_trainer_level_boost");
                        AddListItem(2, "option_wild_rng");
                        AddListItem(3, "option_wild_live_rng");
                        AddListItem(4, "option_wild_level_boost");
                        AddListItem(5, "option_scripted_rng");
                        AddListItem(6, "option_next");
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: 
                            {
                                page--;
                            }
                            case 1: ToggleSetting(8);
                            case 2: ToggleSetting(9);
                            case 3: ToggleSetting(10);
                            case 4: ToggleSetting(11);
                            case 5: ToggleSetting(12);
                            case 6:
                            {
                                page++;
                            }
                        }
                    }
                    case 3:
                    {
                        AddListItem(0, "option_back");
                        AddListItem(1, "option_gift_rng");
                        AddListItem(2, "option_model_rng");
                        AddListItem(3, "option_model_hide_starters");
                        AddListItem(4, "option_permadeath");
                        AddListItem(5, "option_route_restriction");
                        AddListItem(6, "option_next");
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: 
                            {
                                page--;
                            }
                            case 1: ToggleSetting(13);
                            case 2: ToggleSetting(14);
                            case 3: ToggleSetting(15);
                            case 4: ToggleSetting(16);
                            case 5: ToggleSetting(17);
                            case 6:
                            {
                                page++;
                            }
                        }
                    }
                    case 4:
                    {
                        AddListItem(0, "option_back");
                        AddListItem(1, "option_level_cap_boost");
                        AddListItem(2, "option_filter_shop_items");
                        // AddListItem(6, "option_next");
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: 
                            {
                                page--;
                            }
                            case 1: ToggleSetting(18);
                            case 2: ToggleSetting(19);
                            // case 3: ToggleSetting(20);
                            // case 4: ToggleSetting(21);
                            // case 5: ToggleSetting(22);
                            // case 6:
                            // {
                            //     page++;
                            // }
                        }
                    }
                }
            }
        }
        default:
            CommandNOP()
    }
}