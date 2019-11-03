#pragma once
#include "core/engine.hpp"

class Unit;
class AI;
class Player;
class Interactable;

#include "inventory.hpp"
#include "mapgen.hpp"

#include <vector>

constexpr uint32_t kTilesize = 96;

//////////////////////////////////////////////////////
class GameState : public State
{
public:
	enum class State {
		GAME,
		CRAFTING
	};

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
	std::vector<Player*>& getPlayers() { return m_players; }
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
	std::vector<AI*> m_AIs;
    std::vector<Player*> m_players;
    std::vector<Interactable*> m_interactables;

    // Textures
    Texture * m_tile_texture;
    Texture * m_ui_texture;
    Texture * m_white_overlay;
    Texture * m_end_turn;
	Texture * m_crafting;
	Texture * m_craftingBackground;
	Texture * m_craftingLeft;
	Texture * m_craftingRight;

    // Game State
	State m_state;
    uint32_t m_camera_x;
    uint32_t m_camera_y;

    Player * m_selected;    // Reference, not owned
    bool m_playerTurn;
    int tempDelta;

    bool m_panning;
    uint32_t m_pan_start_x;
    uint32_t m_pan_start_y;
    int32_t m_pan_start_cam_x;
    int32_t m_pan_start_cam_y;

	ItemType m_craftingIndex;

    // Helper functions
    void StartTurn();
    void EndTurn();

	// Render helper functions
	void renderPlayerPortraits();
};