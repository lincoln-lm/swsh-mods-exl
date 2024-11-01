#pragma once
#include "symbols.hpp"

// requires vtable_offset
template<typename T>
void* getClassInheritance() {
    static_assert(std::is_base_of_v<WorldObject, T>);
    // GetInstanceInheritance() is at vtable_offset + 0x30
    return external_absolute<void*>(read_main<u64>(T::vtable_offset + 0x30));
}

namespace PersonalInfo {
    bool isInGame(u16 species, u16 form) {
        FetchInfo(species, form);
        u8* current_personal_info = read_main<u8*>(static_CurrentPersonalInfo_offset);
        return ((*(current_personal_info + 0x31)) >> 6) & 1;
    }
}

hashed_string_t getFNV1aHashedString(const char* string) {
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

template <class...> constexpr std::false_type always_false{};

namespace Field {
    template<typename T>
    constexpr u64 getPushOffset() {
        if constexpr (std::is_same_v<T, FlatbufferObjects::FieldBallItem>) return PushFieldBallItem_offset;
        else if constexpr (std::is_same_v<T, FlatbufferObjects::NestHoleEmitter>) return PushNestHoleEmitter_offset;
        else if constexpr (std::is_same_v<T, FlatbufferObjects::UnitObject>) return PushUnitObject_offset;
        else if constexpr (std::is_same_v<T, FlatbufferObjects::PokemonModel>) return PushPokemonModel_offset;
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