#include "unit.hpp"

Unit::Unit(int x, int y)
    : GridItem(new Texture("res/enemy.png"), 64, 128, x, y)
    , m_movesLeft(6)
    , m_attacked(false)
    , m_maxHealth(10)
    , m_currentHealth(10)
{}

Unit::Unit(Texture * tex, int w, int h, int x, int y)
    : GridItem(tex, w, h, x, y)
    , m_movesLeft(6)
    , m_attacked(false)
    , m_maxHealth(10)
    , m_currentHealth(10)
{}

void Unit::StartTurn()
{
    m_movesLeft = 6;
    m_attacked = false;
}

void Unit::RenderHealth(int cam_x, int cam_y, int tilesize) const
{
    static Texture red("res/red.png");
    red.render(
        m_pos_x * tilesize + 10 - cam_x,
        (m_pos_y + 1) * tilesize - m_textureHeight - 15 - cam_y,
        tilesize - 20,
        15
    );
    static Texture green("res/green.png");
    green.render(
        m_pos_x * tilesize + 10 - cam_x,
        (m_pos_y + 1) * tilesize - m_textureHeight - 15 - cam_y,
        static_cast<int>((tilesize - 20) * static_cast<float>(m_currentHealth) / static_cast<float>(m_maxHealth)),
        15
    );
}

void Unit::TakeDamage(int damage)
{
    m_currentHealth -= damage;
    if (m_currentHealth < 0) m_currentHealth = 0;
}