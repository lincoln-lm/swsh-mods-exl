#pragma once
#include "symbols.hpp"

namespace PersonalInfo {
    bool isInGame(u16 species, u16 form) {
        FetchInfo(species, form);
        u8* current_personal_info = read_main<u8*>(static_CurrentPersonalInfo_offset);
        return ((*(current_personal_info + 0x31)) >> 6) & 1;
    }
}

namespace Field {
    template<typename T>
    constexpr u64 getPushOffset() {
        if (std::is_same_v<T, FieldBallItem>) return PushFieldBallItem_offset;
        if (std::is_same_v<T, NestHoleEmitter>) return PushNestHoleEmitter_offset;
        assert(false);
    }
    FieldSingleton* getFieldSingleton() {
        return read_main<FieldSingleton*>(FieldSingleton_offset);
    }
    std::vector<FieldObject*> getFieldObjects() {
        return getFieldSingleton()->field_objects;
    }
    template<typename T>
    u64 pushFieldObject(const T* flatbuffer) {
        return external<u64>(getPushOffset<T>(), getFieldSingleton(), flatbuffer);
    }
    bool checkInheritance(FieldObject* obj, void* inheritance) {
        void* obj_inheritance = obj->GetInheritance();
        while (obj_inheritance != nullptr) {
            if (obj_inheritance == inheritance) return true;
            obj_inheritance = *reinterpret_cast<void**>(obj_inheritance);
        }
        return false;
    }
}