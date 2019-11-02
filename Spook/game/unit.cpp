#include "unit.hpp"

Unit::Unit(int x, int y)
    : m_texture(new Texture("res/unit.png"))
    , m_unitWidth(64)
    , m_unitHeight(128)
    , m_pos_x(x)
    , m_pos_y(y)
{}

Unit::Unit(Texture * tex, int w, int h, int x, int y)
    : m_texture(tex)
    , m_unitWidth(w)
    , m_unitHeight(h)
    , m_pos_x(x)
    , m_pos_y(y)
{}

void Unit::Render(int cam_x, int cam_y, int tilesize) const
{
    m_texture->render(
        m_pos_x * tilesize - cam_x + (tilesize - m_unitWidth) / 2, 
        m_pos_y * tilesize - cam_y + tilesize - m_unitHeight, 
        m_unitWidth, 
        m_unitHeight);
}