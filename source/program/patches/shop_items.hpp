#include "external.hpp"
#include "lib.hpp"
#include "savefile.hpp"
#include "schemas/shop_inventory_generated.h"
#include "symbols.hpp"
#include "util.hpp"

// whitelist pokeballs
constexpr std::array<u16, 27> WHITELISTED_SHOP_ITEMS {
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

    851
};

HOOK_DEFINE_TRAMPOLINE(FilterShopItems) {
    static inline std::vector<u8> template_data = {};

    static void Callback(u64 table, u64* hash, u8 index) {
        Orig(table, hash, index);
        if (!save_file.filter_shop_items.enabled) return;
        const ShopInventory** inventory_location = reinterpret_cast<const ShopInventory**>(table + 0x60);
        const ShopInventory* original = *inventory_location;
        if (original == nullptr) return;

        std::vector<int> items;

        for (u32 i = 0; i < original->items()->size(); i++) {
            auto item = original->items()->Get(i);
            if (std::find(WHITELISTED_SHOP_ITEMS.begin(), WHITELISTED_SHOP_ITEMS.end(), item) != WHITELISTED_SHOP_ITEMS.end()) {
                items.push_back(item);
            }
        }
        if (items.size() == 0) {
            items.push_back(106);
        }

        flatbuffers::FlatBufferBuilder builder;
        builder.Finish(
            CreateShopInventory(
                builder,
                builder.CreateVector(items.data(), items.size())));
        u8* data_ptr = builder.GetBufferPointer();
        size_t data_size = builder.GetSize();
        template_data.clear();
        template_data.insert(template_data.end(), data_ptr, data_ptr + data_size);

        *inventory_location = flatbuffers::GetRoot<ShopInventory>(template_data.data());
    }
};

void install_shop_items_patch()
{
    // TODO: symbol
    // function that returns the shop flatbuffer from the hash
    FilterShopItems::InstallAtOffset(0x14e2f50 - VER_OFF);
}