// TODO: sync std with swsh-pawn-decomp

native _Suspend(delay);

enum GameVersion {
    Any = 0,
    Sword = 44,
    Shield = 45
}

native GetFnvHash64(string[]);
native FadeIn_(arg0, hash);
native FadeOut_(arg0, hash, arg2, arg3);
native FadeWait_();
native CommandNOP();
native FlagGet(hash);
native PokePartyCanEvolve(species);
native ReserveScript(hash);
native FlagSet(hash);
native PokePartyCallEvolve(species, form);
native FlagReset(hash);
native WorkGet(hash);
native WorkSet(hash, value);
native GetPublicRand(maximum);
native AddRecord_(arg0, arg1);
native IncRecord_(arg0);
native DeleteFieldObject_(hash);
native AddFieldObject_(hash);
native VanishFlagGet(hash);
native GameVersion:RomGetVersion();
native PlayerGetZoneID();
native MsgWin_(arg0, arg1, arg2, bubble_type, arg4, arg5, arg6, arg7, arg8, arg9);
native bool:IsMsgWinEnd_();
native ShowMsgWinCursor_(arg0);
native ABKeyWait_();
native SoundPostEvent(sound[]);
native MsgWinClose();
native ExtraMsgLoad(msg[]);
native ExtraMsgIsLoaded_();
native ExtraMsgUnload();
native PG_WordSetRegister(arg0, arg1, arg2, arg3);
native UpdateWalletWindow_(arg0);
native AddWatt_(delta);
native WordSetNumber(arg0, arg1, arg2, arg3);
native TempWorkGet(idx);
native TempWorkSet(idx, value);
native GetWildBattleResult_(out[]);
native PokePartyGetCount(arg0, arg1);
native GetLastSymbolEncountParam(out[]);
native CallBattleLose_();
native SoundIsPlaying(sound);
native StartTurnAround_(arg0, arg1, arg2, arg3, arg4, arg5)
native CheckActionCommandRunningState_(action_command);
native DeletePlayingActionCommand_(action_command);
native SetAnimationStateTrigger_(arg0, arg1[]);
native SetAnimationStateIntParameter_(arg0, arg1[], arg2);
native GetAnimationStateIntParameter_(arg0, arg1[]);
native bool:IsExistAnimationComponent_(arg0);
native bool:IsEndCharacterMotion_(arg0);
native bool:IsEndPokemonMotion_(arg0);
native ResetAnimationState_(arg0);
native AddParallelCommandControlFacial_(arg0, arg1[], arg2[], arg3[], arg4[], arg5[], arg6, arg7, arg8[], arg9, arg10, arg11[], arg12, arg13, arg14[], arg15, arg16)
native DisableFieldObjectLookAt_(arg0, arg1);
native bool:IsPokemonObject(arg0);
native bool:IsExistPokemonMotion(arg0);
native SetNPCDefaultMotion(arg0);
native MsgWinEmpty_();
native bool:MsgWinLoadWait_();
native ListMenuAdd_(arg0, arg1, arg2, arg3);
native ListMenuStart_Seq(arg0, arg1, arg2, arg3, arg4);
native YesNoWin_(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
native PokePartyGetParam(arg0, arg1, arg2);
native EasyTalkPlayer(arg0, arg1, arg2);
native EasyTalkCharacter(arg0, arg1, arg2, arg3, arg4, arg5);
native FindParallelWait(arg0, arg1[]);
native EasyTalkTerminate(arg0);
native bool:IsPlayerRideBicycle();
native bool:IsAnimationStateName_(arg0, arg1[]);
native bool:ItemHaveNeverAdded(arg0);
native ItemOpenDescWindow(arg0);
native ItemCloseDescWindow();
native bool:ItemAddCheck(arg0, arg1);
native ItemAdd(arg0, arg1);
native bool:ItemIsWazaMachine(item);
native bool:ItemIsEventItem(item);
native ItemGetCategory(item);
native GetPocketNumberFromItemNumber_(item);
native GetTargetFieldObjectID();
native SetPlayerTentColor(color);
native GetCookedTypeCount();


#pragma unused g_mode
#pragma unused g_skip_point
#pragma unused g_skip_point_end
#pragma unused g_skip_point_max
public g_mode = 0;
public g_skip_point = 0;
public g_skip_point_end = 99;
public g_skip_point_max = 99;

stock Suspend(delay)
{
    _Suspend(delay);
    sleep(0);
}

stock SuspendN(arg0)
{
    if (arg0 <= 0) {
        return;
    }
    for (new a = 0; a < arg0; a++) {
        Suspend(1);
    }
}

stock RandMax(max, arg1)
{
    if (arg1) {
        return GetPublicRand(max);
    }
    return GetPublicRand(max - 1);
}

stock RandRange(arg0, arg1)
{
    return GetPublicRand(arg1 - arg0) + arg0;
}

stock AddRecord(arg0, arg1)
{
    return AddRecord_(arg0, arg1);
}

stock IncRecord(arg0)
{
    return IncRecord_(arg0);
}

stock FadeIn(arg0, color[])
{
    FadeIn_(arg0, GetFnvHash64(color));
}

stock FadeOut(arg0, color[], arg2, arg3)
{
    FadeOut_(arg0, GetFnvHash64(color), arg2, arg3);
}

stock FadeWait()
{
    // functionally "while (true)" but inserts an extra jump instruction
    // at the start of the loop
    for (;;) {
        if (FadeWait_() != 0) {
            return;
        }
        Suspend(1);
    }
}

stock AddFieldObject(hash)
{
    return AddFieldObject_(hash);
}

stock DeleteFieldObject(hash)
{
    return DeleteFieldObject_(hash);
}

stock ABKeyWait()
{
    return ABKeyWait_();
}

stock CopyString(destination[], source[], size)
{
    for (new i = 0; i < size; i++) {
        if (!source[i]) {
            return;
        }
        destination[i] = source[i];
    }
}

stock WaitForSoundToFinish(sound)
{
    new max_timeout = 3600;
    new time_waited = 0;
    for (;;) {
        if (!SoundIsPlaying(sound)) {
            break;
        }
        time_waited++;
        if (time_waited >= max_timeout) {
            break;
        }
        Suspend(1);
    }
}

stock StartTurnAround(arg0, arg1, arg2, arg3, arg4, arg5)
{
    return StartTurnAround_(arg0, arg1, arg2, arg3, arg4, arg5);
}

stock CleanUpRunningActionCommand(action_command)
{
    if (TestIsPokemonObject(action_command)) 
    {
        if (!TestIsExistPokemonMotion(action_command))
        {
            return;
        }
    }
    for (;;) {
        if (!CheckActionCommandRunningState_(action_command)) {
            DeletePlayingActionCommand_(action_command);
            return;
        }
        Suspend(1);
    }
}

stock SetAnimationStateTrigger(arg0, arg1[])
{
    return SetAnimationStateTrigger_(arg0, arg1);
}

stock SetAnimationStateIntParameter(arg0, arg1[], arg2)
{
    return SetAnimationStateIntParameter_(arg0, arg1, arg2);
}

stock GetAnimationStateIntParameter(arg0, arg1[])
{
    return GetAnimationStateIntParameter_(arg0, arg1);
}

stock WaitForAnimation(arg0)
{
    if (!IsExistAnimationComponent_(arg0)) {
        return;
    }
    SuspendN(1);
    for (new i = 0;;) {
        if (!TestIsPokemonObject(arg0)) {
            if (IsEndCharacterMotion_(arg0)) {
                return;
            }
        } else {
            if (IsEndPokemonMotion_(arg0)) {
                return;
            }
        }
        i++;
        if (i > 300) {
            ResetAnimationState(arg0);
            return;
        }
        Suspend(1);
    }
}

stock ResetAnimationState(arg0)
{
    ResetAnimationState_(arg0);
}

stock AddParallelCommandControlFacial(field_object, arg1[], arg2[], arg3[], arg4[], arg5, arg6)
{
    if (TempWorkGet(14) == true) {
        return;
    }
    if (TestIsPokemonObject(field_object)) {
        if (arg5 == -1) {
            arg5 = 0;
        }
        if (arg6 == -1) {
            arg6 = 0;
        }
        AddParallelCommandControlFacial_(field_object, arg1, arg2, arg3, arg4, "eye01", 0, arg5, "mouth01", 0, arg6, "", -1, -1, "", 0, 0);
    } else {
        if (arg5 == -1) {
            arg5 = 1;
        }
        if (arg6 == -1) {
            arg6 = 1;
        }
        AddParallelCommandControlFacial_(field_object, arg1, arg2, arg3, arg4, "face", 0, arg5, "mouth", 0, arg6, "", -1, -1, "eye_switch", 0, 1);
    }
}

stock SetTemp14()
{
    TempWorkSet(14, 1);
}

stock ClearTemp14()
{
    TempWorkSet(14, 0);
}

stock DisableFieldObjectLookAt(arg0, arg1)
{
    DisableFieldObjectLookAt_(arg0, arg1);
}

stock bool:TestIsPokemonObject(arg0)
{
    return IsPokemonObject(arg0);
}

stock bool:TestIsExistPokemonMotion(arg0)
{
    return IsExistPokemonMotion(arg0);
}

stock DoSetNPCDefaultMotion(arg0)
{
    return SetNPCDefaultMotion(arg0);
}

stock ShowMessageWindow(arg0, msg_hash, bubble_type_0, bubble_type_1, flags, arg5, arg6)
{
    new flag0 = flags & (1 << 0);
    new flag2 = flags & (1 << 2);
    new flag4 = flags & (1 << 4);
    new flag7 = flags & (1 << 3);
    new bubble_type = 0;
    switch (bubble_type_0) {
        case 100:
            switch (bubble_type_1) {
                case 0: bubble_type = 32
                case 1: bubble_type = 30
                case 2: bubble_type = 31
                case 3: bubble_type = 35
                case 4: bubble_type = 33
                case 5: bubble_type = 34
                default: bubble_type = 35
            }
        case 101:
            switch (bubble_type_1) {
                case 0: bubble_type = 38
                case 1: bubble_type = 36
                case 2: bubble_type = 37
                case 3: bubble_type = 41
                case 4: bubble_type = 39
                case 5: bubble_type = 40
                default: bubble_type = 41
            }
        case 102:
            switch (bubble_type_1) {
                case 0: bubble_type = 63
                case 1: bubble_type = 64
                case 2: bubble_type = 65
                case 3: bubble_type = 60
                case 4: bubble_type = 61
                case 5: bubble_type = 62
                default: bubble_type = 60
            }
        default: bubble_type = bubble_type_0
    }
    MsgWin_(arg0, 0, msg_hash, bubble_type, flag0, flag2, flag4, flag7, arg5, arg6);
    if (!(flags & (1 << 1))) {
        WaitForMessageEnd();
    }
}

stock ShowMessageWindow_1(msg_hash, bubble_type_0, flags, bubble_type_1)
{
    ShowMessageWindow(0, msg_hash, bubble_type_0, bubble_type_1, flags, 0, 0);
}

stock GetMouthAnimation(arg0)
{
    new animations[10] = {
        1,
        3,
        4,
        2,
        3,
        2,
        4,
        1,
        1,
        1,
    }
    new animation_param = GetAnimationStateIntParameter(arg0, "mouth");
    if (animation_param == 0) {
        return 1;
    }
    return animations[animation_param-1];
}

stock ShowMouthBasedMessageWindow(arg0, msg_hash, arg2, bubble_type_0, flags, bubble_type_1, arg6)
{
    if (arg2 == -1) {
        arg2 = GetMouthAnimation(arg0);
    }
    ShowMessageWindow(arg0, msg_hash, bubble_type_0, bubble_type_1, flags, arg2, arg6);
}

stock ShowMessageWindow_2(msg_hash, flags, bubble_type)
{
    ShowMessageWindow_1(msg_hash, 102, flags, bubble_type);
}

stock ShowMessageWindow_3(msg_hash, flags, bubble_type)
{
    ShowMessageWindow_2(msg_hash, flags | (1 << 0), bubble_type);
}

stock ShowMessageWindow_4(msg_hash, flags)
{
    ShowMessageWindow_1(msg_hash, 47, flags | (1 << 2), 3);
}

stock WaitForMessageEnd()
{
    for (;;)
    {
        if (IsMsgWinEnd_() == true) {
            return;
        }
        Suspend(1);
    }
}

stock FinishMessage(should_play_sound)
{
    WaitForMessageEnd();
    ShowMsgWinCursor_(true);
    ABKeyWait_();
    ShowMsgWinCursor_(false);
    if (should_play_sound) {
        SoundPostEvent("Play_UI_common_decide");
    }
}

stock CloseMessageWindow()
{
    MsgWinClose();
}

stock WaitUntilMessageWindowIsLoaded(arg0, arg1, arg2)
{
    MsgWinEmpty_()
    for (;;) {
        if (MsgWinLoadWait_() == true) {
            break;
        }
        Suspend(1);
    }
    ListMenuAdd_(0, arg0, arg1, arg2);
}

stock RequestListInput(arg0, arg1, arg2, arg3)
{
    ListMenuStart_Seq(arg0, 0, arg1, arg2, arg3);
    return TempWorkGet(12);
}

stock bool:RequestYesNo(arg0, arg1, arg2, arg3, arg4, arg5)
{
    YesNoWin_(arg0, 0, arg1, arg2, arg3, arg4, arg5);
    return GetMessageInputBool();
}

stock bool:GetMessageInputBool()
{
    return TempWorkGet(12) == 0;
}

stock LoadExtraMessage(msg[])
{
    ExtraMsgLoad(msg);
}

stock WaitUntilExtraMessageIsLoaded()
{
    for (;;) {
        if (ExtraMsgIsLoaded_()) {
            break;
        } else {
            Suspend(1);
        }
    }
}

stock UnloadExtraMessage()
{
    ExtraMsgUnload();
}

stock PG_WordSetRegisterPlayerName(arg0)
{
    PG_WordSetRegister(1, arg0, 0, 0);
}

stock PG_WordSetRegisterPokemonType(arg0, arg1)
{
    PG_WordSetRegister(6, arg0, arg1, 0);
}

stock PG_WordSetRegisterItem(arg0, arg1, arg2)
{
    PG_WordSetRegister(11, arg0, arg1, arg2);
}

stock PG_WordSetRegisterPokemonMove(arg0, arg1)
{
    PG_WordSetRegister(10, arg0, arg1, 0);
}

stock PG_WordSetRegisterBagPocket(arg0, arg1)
{
    PG_WordSetRegister(15, arg0, arg1, 0);
}

stock PG_WordSetRegisterPokemonGender(arg0, arg1)
{
    PG_WordSetRegister(17, arg0, arg1, 0);
}

stock GetNextValidPartyMember(alive, party_index)
{
    new default_value = -1;
    new party_count = PokePartyGetCount(0, 0);
    while (party_index < party_count) {
        switch (alive) {
            // 8 = isEgg; 2 = current hp
            case 1:
                if (!PokePartyGetParam(party_index, 8, 0) && PokePartyGetParam(party_index, 2, 0) > 0) {
                    return party_index;
                }
            case 0:
                if (!PokePartyGetParam(party_index, 8, 0)) {
                    return party_index;
                }
        }
        party_index += 1;
    }
    return default_value;
}

stock UpdateWalletWindow(arg0)
{
    UpdateWalletWindow_(arg0);
}

stock AddWatt(delta)
{
    return AddWatt_(delta);
}

enum WildBattleResult {
    Defeat = 0,
    Win = 1,
    Unk2 = 2,
    RunAway = 3,
    Unk4 = 4,
    Capture = 5,
    Unk6 = 6,
}

stock WildBattleResult:GetWildBattleResult()
{
    new data[3];
    GetWildBattleResult_(data);
    return data[0];
}

stock bool:HasLostLastWildBattle()
{
    if (GetWildBattleResult() == WildBattleResult:Unk2) {
        if (!PokePartyGetCount(1, 0)) {
            return true;
        }
    }
    return GetWildBattleResult() == WildBattleResult:Defeat;
}

stock GetLastSymbolEncoutSpawner()
{
    new data[4];
    GetLastSymbolEncountParam(data);
    return data[2];
}

stock LoseBattle()
{
    CallBattleLose_();
    SuspendN(1);
}