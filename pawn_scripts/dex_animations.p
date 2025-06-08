#include "std.inc"

native SetAnimation_(hash);

SetAnimation(str[]) {
    SetAnimation_(GetFnvHash64(str));
}

AddListItem(index, identifier[]) {
    // TODO: fix name lol
    // WaitUntilMessageWindowIsLoaded
    WaitUntilMessageWindowIsLoaded(index, GetFnvHash64(identifier), 1);
}

main()
{
    switch (g_mode) {
        case "dex_dialog":
        {
            new page = 0;
            while (true) {
                ShowMessageWindow(0, GetFnvHash64("ask_animation"), 100, 3, 1, 0, 0);
                switch (page) {
                    case 0:
                    {
                        AddListItem(0, "to_ba01_landC01");
                        AddListItem(1, "to_ba01_land_state");
                        AddListItem(2, "to_ba02_megaappeal01");
                        AddListItem(3, "to_ba02_roar01");
                        AddListItem(4, "to_ba10_waitA01");
                        AddListItem(5, "to_ba10_waitB01");
                        AddListItem(6, "option_next");
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: {SetAnimation("to_ba01_landC01"); break;}
                            case 1: {SetAnimation("to_ba01_land_state"); break;}
                            case 2: {SetAnimation("to_ba02_megaappeal01"); break;}
                            case 3: {SetAnimation("to_ba02_roar01"); break;}
                            case 4: {SetAnimation("to_ba10_waitA01"); break;}
                            case 5: {SetAnimation("to_ba10_waitB01"); break;}
                            case 6: page++;
                        }
                    }
                    case 1:
                    {
                        AddListItem(0, "option_back");
                        AddListItem(1, "to_ba20_buturi01");
                        AddListItem(2, "to_ba20_buturi02");
                        AddListItem(3, "to_ba20_buturi03");
                        AddListItem(4, "to_ba20_buturi04");
                        AddListItem(5, "to_ba21_tokusyu01");
                        AddListItem(6, "option_next");
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: page--;
                            case 1: {SetAnimation("to_ba20_buturi01"); break;}
                            case 2: {SetAnimation("to_ba20_buturi02"); break;}
                            case 3: {SetAnimation("to_ba20_buturi03"); break;}
                            case 4: {SetAnimation("to_ba20_buturi04"); break;}
                            case 5: {SetAnimation("to_ba21_tokusyu01"); break;}
                            case 6: page++;
                        }
                    }
                    case 2:
                    {
                        AddListItem(0, "option_back");
                        AddListItem(1, "to_ba21_tokusyu02");
                        AddListItem(2, "to_ba21_tokusyu03");
                        AddListItem(3, "to_ba21_tokusyu04");
                        AddListItem(4, "to_ba30_damageS01");
                        AddListItem(5, "to_ba41_down01");
                        AddListItem(6, "option_next");
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: page--;
                            case 1: {SetAnimation("to_ba21_tokusyu02"); break;}
                            case 2: {SetAnimation("to_ba21_tokusyu03"); break;}
                            case 3: {SetAnimation("to_ba21_tokusyu04"); break;}
                            case 4: {SetAnimation("to_ba30_damageS01"); break;}
                            case 5: {SetAnimation("to_ba41_down01"); break;}
                            case 6: page++;
                        }
                    }
                    case 3:
                    {
                        AddListItem(0, "option_back");
                        AddListItem(1, "to_ba50_wideuse01");
                        AddListItem(2, "to_ba50_wideuse02");
                        AddListItem(3, "to_ba50_wideuse03");
                        AddListItem(4, "to_eye_blink01");
                        AddListItem(5, "to_eye_blink02");
                        AddListItem(5, "to_eye_blink03");
                        new response = RequestListInput(1, 0, 1, 0);
                        CloseMessageWindow();
                        switch (response) {
                            case 0: page--;
                            case 1: {SetAnimation("to_ba50_wideuse01"); break;}
                            case 2: {SetAnimation("to_ba50_wideuse02"); break;}
                            case 3: {SetAnimation("to_ba50_wideuse03"); break;}
                            case 4: {SetAnimation("to_eye_blink01"); break;}
                            case 5: {SetAnimation("to_eye_blink02"); break;}
                            case 6: {SetAnimation("to_eye_blink03"); break;}
                        }
                    }
                }
            }
        }
        default:
            CommandNOP()
    }
}