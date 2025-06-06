#pragma once

#include "symbols.hpp"
#include <format>
#include "flatbuffers/minireflect.h"

Vec3f operator+(const Vec3f& lhs, const Vec3f& rhs) {
    Vec3f result(0);
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    return result;
}

Vec3f operator-(const Vec3f& lhs, const Vec3f& rhs) {
    Vec3f result(0);
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    return result;
}

void debug_message_window(std::string title, std::string description) {
#ifdef DEBUG
    nn::err::ApplicationErrorArg err(
        nn::err::MakeErrorCode(nn::err::ErrorCodeCategoryType::unk1, 0x420), title.c_str(),
        description.c_str(),
        nn::settings::LanguageCode::Make(nn::settings::Language::Language_English)
    );
    nn::err::ShowApplicationError(err);
#endif
}

template<typename... Args>
void debug_message(std::format_string<Args...> fmt, Args&&... args) {
#ifdef DEBUG
    std::string formatted = std::format(fmt, std::forward<Args>(args)...);
    debug_message_window(formatted, "");
#endif
}

namespace FlatbufferObjects {
    template<typename T>
    inline std::string ObjectToString(
        const T* object,
        bool multi_line = false,
        bool vector_delimited = true,
        const std::string &indent = "",
        bool quotes = false
    ) {
        flatbuffers::ToStringVisitor tostring_visitor(multi_line ? "\n" : " ", quotes, indent, vector_delimited);
        flatbuffers::IterateObject(reinterpret_cast<const u8*>(object), T::MiniReflectTypeTable(), &tostring_visitor);
        return tostring_visitor.s;
    }
}

namespace V3f {
    float magnitude(const Vec3f& v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    float distance(const Vec3f& lhs, const Vec3f& rhs) {
        return magnitude(lhs - rhs);
    }
}

void* getClassInheritance(u64 vtable_offset) {
    // GetInstanceInheritance() is at vtable_offset + 0x30
    return external_absolute<void*>(read_main<u64>(vtable_offset + 0x30));
}

// requires ::vtable_offset
template<typename T>
void* getClassInheritance() {
    static_assert(std::is_base_of_v<WorldObject, T>);
    return getClassInheritance(T::vtable_offset);
}

namespace PersonalInfo {
    bool isInGame(u16 species, u16 form) {
        FetchInfo(species, form);
        u8* current_personal_info = read_main<u8*>(static_CurrentPersonalInfo_offset);
        return ((*(current_personal_info + 0x31)) >> 6) & 1;
    }
}

constexpr hashed_string_t getFNV1aHashedString(const char* string) {
    hashed_string_t hashed_string = {
        .hash = 0xcbf29ce484222645,
        .string = string,
        .length = strlen(string),
        .unk = 0
    };
    for (size_t i = 0; i < strlen(string); i++) {
        hashed_string.hash = (hashed_string.hash ^ string[i]) * 0x100000001b3;
    }
    return hashed_string;
}

consteval hashed_string_t getConstFNV1aHashedString(const char* string) {
    return getFNV1aHashedString(string);
}

template <class...> constexpr std::false_type always_false{};

namespace Field {
    template<typename T>
    constexpr u64 getPushOffset() {
        if constexpr (std::is_same_v<T, FlatbufferObjects::FieldBallItem>) return PushFieldBallItem_offset;
        else if constexpr (std::is_same_v<T, FlatbufferObjects::NestHoleEmitter>) return PushNestHoleEmitter_offset;
        else if constexpr (std::is_same_v<T, FlatbufferObjects::UnitObject>) return PushUnitObject_offset;
        else if constexpr (std::is_same_v<T, FlatbufferObjects::PokemonModel>) return PushPokemonModel_offset;
        else if constexpr (std::is_same_v<T, FlatbufferObjects::GimmickEncountSpawner>) return PushGimmickEncountSpawner_offset;
        else static_assert(always_false<T>);
    }
    FieldSingleton* getFieldSingleton() {
        return read_main<FieldSingleton*>(FieldSingleton_offset);
    }
    std::vector<FieldObject*> getFieldObjects() {
        return getFieldSingleton()->field_objects;
    }
    template<typename T>
    FieldObject* pushFieldObject(const T* flatbuffer) {
        return external<FieldObject*>(getPushOffset<T>(), getFieldSingleton(), flatbuffer);
    }
    bool checkInheritance(FieldObject* obj, void* inheritance) {
        void* obj_inheritance = obj->GetInstanceInheritance();
        while (obj_inheritance != nullptr) {
            if (obj_inheritance == inheritance) return true;
            obj_inheritance = *reinterpret_cast<void**>(obj_inheritance);
        }
        return false;
    }
}

constexpr std::array<u16, 100> VALID_TMS {
    1230,  328,  329,  330,  331,  332,  333,  334,  335,  336,
     337,  338,  339,  340,  341,  342,  343,  344,  345,  346,
     347,  348,  349,  350,  351,  352,  353,  354,  355,  356,
     357,  358,  359,  360,  361,  362,  363,  364,  365,  366,
     367,  368,  369,  370,  371,  372,  373,  374,  375,  376,
     377,  378,  379,  380,  381,  382,  383,  384,  385,  386,
     387,  388,  389,  390,  391,  392,  393,  394,  395,  396,
     397,  398,  399,  400,  401,  402,  403,  404,  405,  406,
     407,  408,  409,  410,  411,  412,  413,  414,  415,  416,
     417,  418,  419,  618,  619,  620,  690,  691,  692,  693
};

// PKNX's GetRandomItemList
constexpr std::array<u16, 517> VALID_HELD_ITEMS {
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    76,
    77,
    79,
    80,
    81,
    82,
    83,
    84,
    85,
    107,
    108,
    109,
    110,
    112,
    116,
    117,
    118,
    119,
    135,
    136,
    213,
    214,
    215,
    217,
    218,
    219,
    220,
    221,
    222,
    223,
    224,
    225,
    228,
    229,
    230,
    231,
    232,
    233,
    234,
    235,
    236,
    237,
    238,
    239,
    240,
    241,
    242,
    243,
    244,
    245,
    246,
    247,
    248,
    249,
    250,
    251,
    252,
    253,
    254,
    255,
    257,
    258,
    259,
    265,
    266,
    267,
    268,
    269,
    270,
    271,
    272,
    273,
    274,
    275,
    276,
    277,
    278,
    279,
    280,
    281,
    282,
    283,
    284,
    285,
    286,
    287,
    288,
    289,
    290,
    291,
    292,
    293,
    294,
    295,
    296,
    297,
    298,
    299,
    300,
    301,
    302,
    303,
    304,
    305,
    306,
    307,
    308,
    309,
    310,
    311,
    312,
    313,
    314,
    315,
    316,
    317,
    318,
    319,
    320,
    321,
    322,
    323,
    324,
    325,
    326,
    485,
    486,
    487,
    488,
    489,
    490,
    491,
    537,
    538,
    539,
    540,
    541,
    542,
    543,
    544,
    545,
    546,
    547,
    564,
    565,
    566,
    567,
    568,
    569,
    570,
    639,
    640,
    644,
    645,
    646,
    647,
    648,
    649,
    650,
    846,
    849,
    879,
    880,
    881,
    882,
    883,
    884,
    904,
    905,
    906,
    907,
    908,
    909,
    910,
    911,
    912,
    913,
    914,
    915,
    916,
    917,
    918,
    919,
    920,
    1103,
    1104,
    1109,
    1110,
    1111,
    1112,
    1113,
    1114,
    1115,
    1116,
    1117,
    1118,
    1119,
    1120,
    1121,
    1122,
    1123,
    1124,
    1125,
    1126,
    1127,
    1128,
    1129,
    1231,
    1232,
    1233,
    1234,
    1235,
    1236,
    1237,
    1238,
    1239,
    1240,
    1241,
    1242,
    1243,
    1244,
    1245,
    1246,
    1247,
    1248,
    1249,
    1250,
    1251,
    1252,
    1253,
    1254,
    1581,
    1582,
    1588,
    1592,
    1604,
    1606,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    492,
    493,
    494,
    495,
    496,
    497,
    498,
    499,
    500,
    576,
    851,
    55,
    56,
    57,
    58,
    59,
    60,
    61,
    62,
    63,
    1580,
    149,
    150,
    151,
    152,
    153,
    154,
    155,
    156,
    157,
    158,
    159,
    160,
    161,
    162,
    163,
    169,
    170,
    171,
    172,
    173,
    174,
    184,
    185,
    186,
    187,
    188,
    189,
    190,
    191,
    192,
    193,
    194,
    195,
    196,
    197,
    198,
    199,
    200,
    201,
    202,
    203,
    204,
    205,
    206,
    207,
    208,
    209,
    210,
    211,
    212,
    686,
    687,
    688,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    54,
    134,
    504,
    591,
    708,
    709,
    852,
    903,
    1579,
    1130,
    1131,
    1132,
    1133,
    1134,
    1135,
    1136,
    1137,
    1138,
    1139,
    1140,
    1141,
    1142,
    1143,
    1144,
    1145,
    1146,
    1147,
    1148,
    1149,
    1150,
    1151,
    1152,
    1153,
    1154,
    1155,
    1156,
    1157,
    1158,
    1159,
    1160,
    1161,
    1162,
    1163,
    1164,
    1165,
    1166,
    1167,
    1168,
    1169,
    1170,
    1171,
    1172,
    1173,
    1174,
    1175,
    1176,
    1177,
    1178,
    1179,
    1180,
    1181,
    1182,
    1183,
    1184,
    1185,
    1186,
    1187,
    1188,
    1189,
    1190,
    1191,
    1192,
    1193,
    1194,
    1195,
    1196,
    1197,
    1198,
    1199,
    1200,
    1201,
    1202,
    1203,
    1204,
    1205,
    1206,
    1207,
    1208,
    1209,
    1210,
    1211,
    1212,
    1213,
    1214,
    1215,
    1216,
    1217,
    1218,
    1219,
    1220,
    1221,
    1222,
    1223,
    1224,
    1225,
    1226,
    1227,
    1228,
    1229,
    86,
    87,
    88,
    89,
    90,
    91,
    92,
    94,
    106,
    571,
    580,
    581,
    582,
    583,
    795,
    796,
    1105,
    1106,
    1107,
    1108,
    1084,
    1085,
    1086,
    1087,
    1088,
    1089,
    1090,
    1091,
    1092,
    1093,
    1094,
    1095,
    1096,
    1097,
    1098,
    1099,
    1256,
    1257,
    1258,
    1259,
    1260,
    1261,
    1262,
    1263,
    1264
};