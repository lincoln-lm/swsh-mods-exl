#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "util.hpp"

HOOK_DEFINE_INLINE(FilterObject) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (ctx->X[0] == 0) return;
        auto obj = reinterpret_cast<Field::FieldObject*>(ctx->X[0]);
        u64 vtable = *reinterpret_cast<u64*>(ctx->X[0]);

        u64 encount_spawner_vtable = main_offset(Field::EncountSpawner::vtable_offset);
        u64 gimmick_spawner_vtable = main_offset(Field::GimmickSpawner::vtable_offset);
        u64 encount_object_vtable = main_offset(Field::EncountObject::vtable_offset);
        u64 field_ball_item_vtable = main_offset(Field::FieldBallItem::vtable_offset);
        u64 field_sparkle_item_vtable = main_offset(Field::FieldSparkleItem::vtable_offset);
        if (
            vtable == encount_spawner_vtable
            || vtable == gimmick_spawner_vtable
            || vtable == encount_object_vtable
            || vtable == field_ball_item_vtable
            || vtable == field_sparkle_item_vtable
        ) {
            Field::DeleteFieldObject(obj->unique_hash);
        }
    }
};

void install_filter_objects_patch() {
    FilterObject::InstallAtOffset(0xd18e44 - VER_OFF);
    FilterObject::InstallAtOffset(0xcbb2f8 - VER_OFF);
}