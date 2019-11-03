#pragma once
#include "core/engine.hpp"

#include <string>

///////////////////////////////////////////////////////////////
enum ItemType
{
    NONE = -1,
    WOOD = 0,
    MEAT,
    CAMPFIRE,
    COUNT
};

///////////////////////////////////////////////////////////////
class ItemDatabase
{
public:
    struct Item
    {
        std::string icon;   // This stores the path to the icon
        std::string name;
    };
    static Item items[ItemType::COUNT];

    struct Recipe
    {
        ItemType item1;
        ItemType item2;
        ItemType item3;
    };
    static Recipe recipes[ItemType::COUNT];
};

///////////////////////////////////////////////////////////////
constexpr uint32_t kMaxInventorySlots = 10;
class Inventory
{
public:
    Inventory();
    ~Inventory();

    bool Full() const;
    bool AddItem(int id);
    bool HasItemsFor(int id);
    void Craft(int id);
    void RemoveItem(int id);

    void Render();
private:
    int data[kMaxInventorySlots];

    // Inventory textures
    Texture * m_backdrop;
};