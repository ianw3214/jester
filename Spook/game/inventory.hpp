#pragma once
#include "core/engine.hpp"

#include <string>

///////////////////////////////////////////////////////////////
enum ItemType
{
    NONE = -1,
    WOOD = 0,
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
    static Item items[COUNT];
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

    void Render();
private:
    int data[kMaxInventorySlots];

    // Inventory textures
    Texture * m_backdrop;
};