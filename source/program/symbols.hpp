#pragma once

#include "external.hpp"

typedef struct {
    u8 data[566];
} encounter_tables_t;

#ifdef VERSION_SHIELD
static const u64 SendCommand_offset = 0xea2190;
static const u64 AuraHandler_offset = 0xdcac10;
static const u64 MainInit_offset = 0xf112b0; // initializes the class holding field objects
#else
static const u64 SendCommand_offset = 0xea2190 - 0x30;
static const u64 AuraHandler_offset = 0xdcac10 - 0x30;
static const u64 MainInit_offset = 0xf112b0 - 0x30;
#endif
static void SendCommand(const char* command) {
    return external<void>(SendCommand_offset, command);
}

namespace EncountObject {
    #ifdef VERSION_SHIELD
    static const u64 FromParams_offset = 0xea2670;
    #else
    static const u64 FromParams_offset = 0xea2670 - 0x30;
    #endif
}

namespace Field {
    #ifdef VERSION_SHIELD
    static const u64 FetchAreaHash_offset = 0xd7e310;
    #else
    static const u64 FetchAreaHash_offset = 0xd7e310 - 0x30;
    #endif

    static u64 FetchAreaHash() {
        return external<u64>(FetchAreaHash_offset);
    }
}

namespace OverworldEncount {
    #ifdef VERSION_SHIELD
    static const u64 GenerateSymbolEncountParam_offset = 0xd050b0;
    static const u64 FetchSymbolEncountTable_offset = 0xd05750;
    static const u64 TryGenerateSymbolEncount_offset = 0xdaf380;
    static const u64 GenerateMainSpec_offset = 0xd311f0;
    #else
    static const u64 GenerateSymbolEncountParam_offset = 0xd050b0 - 0x30;
    static const u64 FetchSymbolEncountTable_offset = 0xd05750 - 0x30;
    static const u64 TryGenerateSymbolEncount_offset = 0xdaf380 - 0x30;
    #endif

    typedef void EncountSpawner;
    
    static bool GenerateSymbolEncountParam(void* param_1, encounter_tables_t* encounter_tables, u64 unused, void* overworld_spec) {
        return external<bool>(GenerateSymbolEncountParam_offset, param_1, encounter_tables, unused, overworld_spec);
    }
    static encounter_tables_t FetchSymbolEncountTable(u64 unused, u64* hash) {
        return external<encounter_tables_t>(FetchSymbolEncountTable_offset, unused, hash);
    }
    static bool TryGenerateSymbolEncount(EncountSpawner* encount_spawner, void* param_2) {
        return external<bool>(TryGenerateSymbolEncount_offset);
    }
}
