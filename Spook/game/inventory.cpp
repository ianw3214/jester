#include "inventory.hpp"

ItemDatabase::Item ItemDatabase::items[COUNT] = {
    { std::string("res/icons/wood.png"), std::string("wood") }
};

Inventory::Inventory()
{
    for (unsigned int i = 0; i < kMaxInventorySlots; ++i)
    {
        data[i] = ItemType::NONE;
    }
    m_backdrop = new Texture("res/inventory.png");
}
Inventory::~Inventory()
{
    
}

bool Inventory::Full() const
{
    for (unsigned int index = 0; index < kMaxInventorySlots; ++index)
    {
        if (data[index] == ItemType::NONE) return false;
    }
    return true;
}

bool Inventory::AddItem(int id)
{
    for (unsigned int index = 0; index < kMaxInventorySlots; ++index)
    {
        if (data[index] == ItemType::NONE)
        {
            data[index] = id;
            return true;
        }
    }
    return false;
}

void Inventory::Render()
{
    constexpr int inv_x = (1280 - 430) / 2;
    constexpr int inv_y = 720 - 52 - 18;
    m_backdrop->render(inv_x, inv_y, 430, 52);

    int x = (1280 - 430) / 2 + 10;
    int y = 720 - 52 - 18 + 10;
    for (unsigned int index = 0; index < kMaxInventorySlots; ++index)
    {
        int id = data[index];
        if (id > ItemType::NONE && id < ItemType::COUNT)
        {
            const ItemDatabase::Item& item = ItemDatabase::items[id];
            Texture icon(item.icon);
            icon.render(x, y);
        }
        x += 32 + 10;
    }
}