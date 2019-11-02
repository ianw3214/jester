#pragma once
#include "core/engine.hpp"
#include "gameitem.hpp"
class GameState;

//////////////////////////////////////////////////////
class Unit
{
public:
    Unit(int x = 0, int y = 0);
    Unit(Texture * tex, int w, int h, int x = 0, int y = 0);
    virtual ~Unit() {}

    void SetGameState(GameState * g) { game = g; }

    uint32_t getX() const { return m_pos_x; }
    uint32_t getY() const { return m_pos_y; }
    uint16_t getMovesLeft() const { return m_movesLeft; }
    bool getAttacked() const { return m_attacked; }

    void StartTurn();

    void Render(int cam_x, int cam_y, int tilesize) const;
    void RenderHealth(int cam_x, int cam_y, int tilesize) const;

    void TakeDamage(int damage);

protected:
    // Unit sprite info
    Texture * m_texture;
    uint32_t m_unitWidth;
    uint32_t m_unitHeight;

    // Unit properties
    int m_maxHealth;
    int m_currentHealth;

    // Position stored in terms of tile position
    uint32_t m_pos_x;
    uint32_t m_pos_y;

    // Unit states
    uint16_t m_movesLeft;
    bool m_attacked;

    // Store a copy of the game
    GameState * game;
};