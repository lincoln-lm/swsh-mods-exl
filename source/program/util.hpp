#pragma once

#include "symbols.hpp"
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

std::tuple<u32, u16> random_species_and_form() {
    // TODO: deterministic rng based on trainer id/spawner hash/area hash & seed
    u32 species;
    u16 form;
    do {
        species = (exl::util::GetRandomU64() % 898) + 1;
        PersonalInfo::FetchInfo(species, 0);
        u32 form_count = PersonalInfo::GetField(PersonalInfo::InfoField::FORM_COUNT);
        form = exl::util::GetRandomU64() % form_count;
    } while (!PersonalInfo::isInGame(species, form));
    return {species, form};
}

s16 random_valid_move_id() {
    // TODO: deterministic rng based on species/form & seed
    // TODO: ensure unique moves?
    MoveIdHolder move_id_holder = {
        .unk = 0,
        .move_id = 0,
    };
    do {
        move_id_holder.move_id = (exl::util::GetRandomU64() % 820) + 1;
    } while (!move_id_holder.IsMoveUsable());
    return move_id_holder.move_id;
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