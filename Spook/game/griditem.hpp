#pragma once
#include "core/engine.hpp"

class GridItem
{
public:
    GridItem(Texture * tex, int w, int h, int x, int y)
        : m_texture(tex)
        , m_textureWidth(w)
        , m_textureHeight(h)
        , m_pos_x(x)
        , m_pos_y(y)
    {}
    virtual ~GridItem() 
    {
        delete m_texture;
    }

    uint32_t getX() const { return m_pos_x; }
    uint32_t getY() const { return m_pos_y; }

    virtual void Render(int cam_x, int cam_y, int tilesize) const {
        m_texture->render(
            m_pos_x * tilesize - cam_x + (tilesize - (int) m_textureWidth) / 2, 
            m_pos_y * tilesize - cam_y + tilesize - (int) m_textureHeight,
            m_textureWidth, 
            m_textureHeight
        );
    }
protected:
    // Unit sprite info
    Texture * m_texture;
    uint32_t m_textureWidth;
    uint32_t m_textureHeight;

    // Position stored in terms of tile position
    uint32_t m_pos_x;
    uint32_t m_pos_y;
};