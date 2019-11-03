#pragma once
#include "core/engine.hpp"

class GameState;
#include <string>

///////////////////////////////////////////////////////////////
enum ItemType
{
    NONE = -1,
    WOOD = 0,
    MEAT,
    CAMPFIRE,
    COOKED_MEAT,
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
    Inventory(GameState * game);
    ~Inventory();

    bool Full() const;
    bool AddItem(int id);
    bool HasItemsFor(int id);
    void Craft(int id);
    void RemoveItem(int id);

    bool HandleClick(int mouse_x, int mouse_y, int screen_width, int screen_height);
    void Render(int screen_width, int screen_height);

    bool Consume(int id);
private:
    int data[kMaxInventorySlots];

    // Inventory textures
    Texture * m_backdrop;

    // Reference to the game
    GameState * game;
};