#include "inventory.hpp"

ItemDatabase::Item ItemDatabase::items[COUNT] = {
    { std::string("res/icons/wood.png"), std::string("wood") },
    { std::string("res/icons/meat.png"), std::string("meat") },
    { std::string("res/icons/campfire.png"), std::string("campfire") },
};

ItemDatabase::Recipe ItemDatabase::recipes[ItemType::COUNT] = {
    { ItemType::NONE, ItemType::NONE, ItemType::NONE },     // Wood
    { ItemType::NONE, ItemType::NONE, ItemType::NONE },     // Meat
    { ItemType::WOOD, ItemType::WOOD, ItemType::NONE },     // Campfire
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

bool Inventory::HasItemsFor(int id)
{
    if (id <= ItemType::NONE || id >= ItemType::COUNT) 
    {
        return false;
    }
    const ItemDatabase::Recipe& recipe = ItemDatabase::recipes[id];
    if (recipe.item1 == ItemType::NONE && recipe.item2 == ItemType::NONE && recipe.item3 == ItemType::NONE)
    {
        return false;
    }
    bool first = recipe.item1 == ItemType::NONE ? true : false;
    bool second = recipe.item2 == ItemType::NONE ? true : false;
    bool third = recipe.item3 == ItemType::NONE ? true : false;
    for (unsigned int index = 0; index < kMaxInventorySlots; ++index)
    {
        if (!first) 
        {
            if (data[index] == recipe.item1)
            {
                first = true;
                continue;
            }
        }
        if (!second) 
        {
            if (data[index] == recipe.item2)
            {
                second = true;
                continue;
            }
        }
        if (!third) 
        {
            if (data[index] == recipe.item3)
            {
                third = true;
                continue;
            }
        }
    }
    return first && second && third;
}

// Assumes already verified that everything that is needed exists in the inventory
void Inventory::Craft(int id)
{
    const ItemDatabase::Recipe& recipe = ItemDatabase::recipes[id];
    RemoveItem(recipe.item1);
    RemoveItem(recipe.item2);
    RemoveItem(recipe.item3);
    AddItem(id);
}

void Inventory::RemoveItem(int id)
{
    for (unsigned int index = 0; index < kMaxInventorySlots; ++index)
    {
        if (data[index] == id)
        {
            data[index] = ItemType::NONE;
            return;
        }
    }
}

void Inventory::Render(int screen_width, int screen_height)
{
    int inv_x = (screen_width - 750) / 2;
    int inv_y = screen_height - 84 - 18;
    m_backdrop->render(inv_x, inv_y, 750, 84);

    int x = (screen_width - 750) / 2 + 10;
    int y = screen_height- 84 - 18 + 10;
    for (unsigned int index = 0; index < kMaxInventorySlots; ++index)
    {
        int id = data[index];
        if (id > ItemType::NONE && id < ItemType::COUNT)
        {
            const ItemDatabase::Item& item = ItemDatabase::items[id];
            Texture icon(item.icon);
            icon.render(x, y);
        }
        x += 64 + 10;
    }
}