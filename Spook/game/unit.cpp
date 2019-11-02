#include "unit.hpp"

Unit::Unit(int x, int y)
    : m_texture(new Texture("res/unit.png"))
    , m_unitWidth(64)
    , m_unitHeight(128)
    , m_pos_x(x)
    , m_pos_y(y)
    , m_movesLeft(6)
    , m_attacked(false)
    , m_maxHealth(10)
    , m_currentHealth(10)
{}

Unit::Unit(Texture * tex, int w, int h, int x, int y)
    : m_texture(tex)
    , m_unitWidth(w)
    , m_unitHeight(h)
    , m_pos_x(x)
    , m_pos_y(y)
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

void Unit::Render(int cam_x, int cam_y, int tilesize) const
{
    m_texture->render(
        m_pos_x * tilesize - cam_x + (tilesize - m_unitWidth) / 2, 
        m_pos_y * tilesize - cam_y + tilesize - m_unitHeight, 
        m_unitWidth, 
        m_unitHeight
    );
}

void Unit::RenderHealth(int cam_x, int cam_y, int tilesize) const
{
    static Texture red("res/red.png");
    red.render(
        m_pos_x * tilesize + 10 - cam_x,
        (m_pos_y + 1) * tilesize - m_unitHeight - 15 - cam_y,
        tilesize - 20,
        15
    );
    static Texture green("res/green.png");
    green.render(
        m_pos_x * tilesize + 10 - cam_x,
        (m_pos_y + 1) * tilesize - m_unitHeight - 15 - cam_y,
        static_cast<int>((tilesize - 20) * static_cast<float>(m_currentHealth) / static_cast<float>(m_maxHealth)),
        15
    );
}

void Unit::TakeDamage(int damage)
{
    m_currentHealth -= damage;
    if (m_currentHealth < 0) m_currentHealth = 0;
}