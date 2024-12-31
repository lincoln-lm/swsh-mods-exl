#include "std.inc"

main()
{
    switch (g_mode) {
        case "custom_message":
        {
            LoadExtraMessage("script/string_format.dat");
            WaitUntilExtraMessageIsLoaded();
            PG_WordSetRegister(0x20, 0, 0, 0);
            ShowMessageWindow_3(15019408012520783250, 0, 0);
            FinishMessage(true);
            UnloadExtraMessage();
        }
        default:
            CommandNOP()
    }
}