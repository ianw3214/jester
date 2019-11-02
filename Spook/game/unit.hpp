#pragma once
#include "core/engine.hpp"
#include "griditem.hpp"
class GameState;

//////////////////////////////////////////////////////
class Unit : public GridItem
{
public:
    Unit(int x = 0, int y = 0);
    Unit(Texture * tex, int w, int h, int x = 0, int y = 0);
    virtual ~Unit() {}

    void SetGameState(GameState * g) { game = g; }

    uint16_t getMovesLeft() const { return m_movesLeft; }
    bool getAttacked() const { return m_attacked; }

    void StartTurn();

    void RenderHealth(int cam_x, int cam_y, int tilesize) const;

    void TakeDamage(int damage);

protected:

    // Unit properties
    int m_maxHealth;
    int m_currentHealth;

    // Unit states
    uint16_t m_movesLeft;
    bool m_attacked;

    // Store a copy of the game
    GameState * game;
};