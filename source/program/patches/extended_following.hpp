#include "lib.hpp"
#include "external.hpp"
#include "symbols.hpp"
#include "config.hpp"

// TODO: other zones? configurable?
// checking area hash is better if distinct zones arent added

const std::unordered_set<u64> galar_wild_area_zones = {
    getFNV1aHashedString("z_wr0101").hash,
    getFNV1aHashedString("z_wr0102").hash,
    getFNV1aHashedString("z_wr0103").hash,
    getFNV1aHashedString("z_wr0104").hash,
    getFNV1aHashedString("z_wr0105").hash,
    getFNV1aHashedString("z_wr0106").hash,
    getFNV1aHashedString("z_wr0107").hash,
    getFNV1aHashedString("z_wr0108").hash,
    getFNV1aHashedString("z_wr0109").hash,
    getFNV1aHashedString("z_wr0110").hash,
    getFNV1aHashedString("z_wr0121").hash,
    getFNV1aHashedString("z_wr0122").hash,
    getFNV1aHashedString("z_wr0131").hash,
    getFNV1aHashedString("z_wr0132").hash,
    getFNV1aHashedString("z_wr0133").hash,
    getFNV1aHashedString("z_wr0134").hash,
    getFNV1aHashedString("z_wr0135").hash,
    getFNV1aHashedString("z_wr0136").hash,
};

HOOK_DEFINE_INLINE(AddFollowingSpawner) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        EXL_ASSERT(global_config.initialized);
        if (global_config.extended_following.active) {
            auto field_objects = Field::getFieldObjects();
            bool following_spawner_exists = std::any_of(
                field_objects.begin(),
                field_objects.end(),
                [](Field::FieldObject* obj) {
                    return reinterpret_cast<u64*>(obj)[0x3B0 / 8] == 0xbef7f34f7649e9ec;
                }
            );
            if (!following_spawner_exists && galar_wild_area_zones.find(Field::FetchZoneHash()) != galar_wild_area_zones.end()) {
                // mimic ioa spawner
                auto obj_name = getFNV1aHashedString("z_wr02onload_SymbolEncountPokemonGimmickSpawner_WR_Turearuki");
                auto empty_string = getFNV1aHashedString("");
                auto player = Field::GetPlayerObject();
                // TODO: find string
                u64 following_hash = 0xbef7f34f7649e9ec;

                flatbuffers::FlatBufferBuilder builder;
                std::vector<flatbuffers::Offset<FlatbufferObjects::GimmickSpawn>> spawn_table;
                for (int i = 0; i < 9; i++) {
                    spawn_table.push_back(
                        FlatbufferObjects::CreateGimmickSpawnDirect(
                            builder,
                            following_hash,
                            "turearuki",
                            empty_string.hash,
                            1,
                            FlatbufferObjects::CreateEncountArea(
                                builder,
                                1,
                                0.0,
                                0.0,
                                0.0,
                                50.0
                            )
                        )
                    );
                }
                builder.Finish(
                    FlatbufferObjects::CreateGimmickEncountSpawner(
                        builder,
                        FlatbufferObjects::CreateGimmickEncountSpawnerInnerDirect(
                            builder,
                            FlatbufferObjects::CreateFieldObject(
                                builder,
                                // spawn it on the player because the player should start at a reasonable place in bounds
                                player->position.x,
                                player->position.y,
                                player->position.z,
                                1.0,
                                1.0,
                                1.0,
                                obj_name.hash,
                                obj_name.hash,
                                empty_string.hash
                            ),
                            3,
                            100,
                            false,
                            true,
                            &spawn_table,
                            FlatbufferObjects::CreateEncountArea(
                                builder,
                                1,
                                0.0,
                                0.0,
                                0.0,
                                500000.0
                            ),
                            FlatbufferObjects::CreateEncountArea(
                                builder,
                                1,
                                0.0,
                                0.0,
                                0.0,
                                500000.0
                            )
                        )
                    )
                );
                Field::pushFieldObject(flatbuffers::GetRoot<FlatbufferObjects::GimmickEncountSpawner>(builder.GetBufferPointer()));
            }
        }
    }
};

void install_extended_following_patch() {
    // TODO: symbols
    AddFollowingSpawner::InstallAtOffset(0xd18e88 - VER_OFF);
}