#include "std.inc"

main()
{
    switch (g_mode) {
        case "debug_msg_1":
        {
            LoadExtraMessage("script/string_format.dat");
            WaitUntilExtraMessageIsLoaded();
            PG_WordSetRegisterPlayerName(0);
            ShowMessageWindow_3(15019408012520783250, 0, 0);
            FinishMessage(true);
            UnloadExtraMessage();
        }
        default:
            CommandNOP()
    }
}