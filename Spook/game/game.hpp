#pragma once
#include "core/engine.hpp"

#include "unit.hpp"
#include "player.hpp"
#include "interactable.hpp"
#include "inventory.hpp"

#include <vector>

constexpr uint32_t kTilesize = 96;

//////////////////////////////////////////////////////
struct Tile
{
    int index;
};

//////////////////////////////////////////////////////
class GameState : public State
{
public:
    GameState();
    ~GameState();

    void init() override;
    void cleanup() override;

    void pause() override {}
    void resume() override {}

    void update() override;
    void render() override;

    bool checkOccupied(unsigned int x, unsigned int y) const;
    Unit * getUnitAt(unsigned int x, unsigned int y);
	Interactable * getInteractable(unsigned int x, unsigned int y);
	Inventory& getInventory() { return m_inventory; }
private:
    // Map data
    uint32_t m_map_width;
    uint32_t m_map_height;
    std::vector<Tile> m_tilemap;

    // Game data
	Inventory m_inventory;
    std::vector<GridItem*> m_items;
    // references, do not own
    std::vector<Unit*> m_units;
    std::vector<Player*> m_players;
    std::vector<Interactable*> m_interactables;

    // Textures
    Texture * m_tile_texture;
    Texture * m_ui_texture;
    Texture * m_white_overlay;
    Texture * m_end_turn;

    // Game State
    uint32_t m_camera_x;
    uint32_t m_camera_y;

    Player * m_selected;
    bool m_playerTurn;
    int tempDelta;

    bool m_panning;
    uint32_t m_pan_start_x;
    uint32_t m_pan_start_y;
    int32_t m_pan_start_cam_x;
    int32_t m_pan_start_cam_y;

    // Helper functions
    void StartTurn();
    void EndTurn();
};