#pragma once
#include "core/engine.hpp"

#include <vector>

constexpr uint32_t kTilesize = 96;

//////////////////////////////////////////////////////
struct Tile
{
    int index;
};

//////////////////////////////////////////////////////
class Unit
{
public:
    enum class InputState
    {
        NONE,
        MOVE,
        ATTACK,
        INVENTORY
    };

    struct PathNode
    {
        uint32_t x;
        uint32_t y;
    };

    Unit(int x = 0, int y = 0);

    void setState(InputState state) { m_inputState = state; }

    uint32_t getX() const { return m_pos_x; }
    uint32_t getY() const { return m_pos_y; }
    InputState getState() const { return m_inputState; }

    void Render(int cam_x, int cam_y) const;

    void Select();
    void Deselect();

private:
    Texture * m_texture;
    uint32_t m_unitWidth;
    uint32_t m_unitHeight;

    // Position stored in terms of tile position
    uint32_t m_pos_x;
    uint32_t m_pos_y;

    // Unit states
    InputState m_inputState;
    std::vector<PathNode> m_movePath;
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
private:
    // Map data
    uint32_t m_map_width;
    uint32_t m_map_height;
    std::vector<Tile> m_tilemap;

    // Game entities
    std::vector<Unit> m_units;

    // Textures
    Texture * m_tile_texture;
    Texture * m_ui_texture;

    // Game State
    uint32_t m_camera_x;
    uint32_t m_camera_y;

    Unit * m_selected;
    bool m_playerTurn;

    bool m_panning;
    uint32_t m_pan_start_x;
    uint32_t m_pan_start_y;
    uint32_t m_pan_start_cam_x;
    uint32_t m_pan_start_cam_y;
};