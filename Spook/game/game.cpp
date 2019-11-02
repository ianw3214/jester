#include "game.hpp"

Unit::Unit(int x, int y)
    : m_texture(new Texture("res/unit.png"))
    , m_unitWidth(64)
    , m_unitHeight(128)
    , m_pos_x(x)
    , m_pos_y(y)
    , m_inputState(InputState::NONE)
{}

void Unit::Render(int cam_x, int cam_y) const
{
    m_texture->render(
        m_pos_x * kTilesize - cam_x + (kTilesize - m_unitWidth) / 2, 
        m_pos_y * kTilesize - cam_y + kTilesize - m_unitHeight, 
        m_unitWidth, 
        m_unitHeight);
}

void Unit::Select()
{
    m_texture->setBlendMode(SDL_BLENDMODE_ADD);
    m_texture->setColourModulation({255, 255, 255});
}
void Unit::Deselect()
{
    m_texture->setBlendMode(SDL_BLENDMODE_BLEND);
    m_texture->setColourModulation({255, 255, 255});
}

GameState::GameState()
    : m_map_width(10)
    , m_map_height(10)
    , m_camera_x(0)
    , m_camera_y(0)
    , m_selected(nullptr)
    , m_playerTurn(true)
    , m_panning(false)
{
    for (unsigned int i = 0; i < m_map_width; ++i)
    {
        for (unsigned int j = 0; j < m_map_height; ++j)
        {
            m_tilemap.push_back({0});
        }
    }

    // Initialize textures
    createFont("ui30", "res/Munro.ttf", 30);
    m_tile_texture = new Texture("res/tile.png");
    m_ui_texture = new Texture("res/ui_base.png");

    // Initialize some units
    m_units.emplace_back();
    m_units.emplace_back(4, 5);
    m_units.emplace_back(7, 6);
}

GameState::~GameState()
{
    
}

void GameState::init()
{

}
void GameState::cleanup()
{

}

void GameState::update()
{
    if (keyPressed(SDL_SCANCODE_ESCAPE)) {
        exit();
    }
    if (leftMousePressed())
    {
        bool handled = false;
        int mouseTileX = (getMouseX() + m_camera_x) / kTilesize;
        int mouseTileY = (getMouseY() + m_camera_y) / kTilesize;
        for (Unit & unit : m_units)
        {
            if (unit.getX() == mouseTileX && unit.getY() == mouseTileY)
            {
                if (m_selected) m_selected->Deselect();
                m_selected = &unit;
                m_selected->Select();
                handled = true;
                break;
            }
        }
        if (!handled)
        {
            if (m_selected) m_selected->Deselect();
            m_selected = nullptr;
            if (!m_panning)
            {
                m_pan_start_x = getMouseX();
                m_pan_start_y = getMouseY();
                m_pan_start_cam_x = m_camera_x;
                m_pan_start_cam_y = m_camera_y;
                m_panning = true;
            }
        }
    }
    if (leftMouseReleased())
    {
        m_panning = false;
    }
    if (leftMouseHeld())
    {
        if (m_panning == true)
        {
            // Calculate the camera difference and apply it
            m_camera_x = m_pan_start_cam_x + m_pan_start_x - getMouseX();
            m_camera_y = m_pan_start_cam_y + m_pan_start_y - getMouseY();
        }
    }
    // UI - mirrored from render code
    if (m_selected)
    {
        // Render the menu to the top right of the selected unit
        const uint32_t x = (m_selected->getX() + 1) * kTilesize - m_camera_x;
        uint32_t curr_y = m_selected->getY() * kTilesize - m_camera_y - 20;

        Vec2 mousePos(getMouseX(), getMouseY());

        Math::Rectangle move(x, curr_y, 140, 30);
        if (Math::isColliding(mousePos, move))
        {
            m_selected->setState(Unit::InputState::MOVE);
        }
        curr_y += 32;
        Math::Rectangle attack(x, curr_y, 140, 30);
        if (Math::isColliding(mousePos, attack))
        {
            m_selected->setState(Unit::InputState::ATTACK);
        }
        curr_y += 32;
        Math::Rectangle inventory(x, curr_y, 140, 30);
        if (Math::isColliding(mousePos, inventory))
        {
            m_selected->setState(Unit::InputState::INVENTORY);
        }
    }
}

void GameState::render()
{
    for (unsigned int y = 0; y < m_map_height; ++y)
    {
        for (unsigned int x = 0; x < m_map_width; ++x)
        {
            const Tile& tile = m_tilemap[y * m_map_height + x];
            if (tile.index == 0)
            {
                m_tile_texture->render(
                    x * kTilesize - m_camera_x, 
                    y * kTilesize - m_camera_y, 
                    kTilesize,
                    kTilesize
                );
            }
        }
    }
    // Draw a grid overlay for EZ UX
    for (unsigned int row = 1; row < m_map_height; ++row)
    {
        QcE::get_instance()->drawLine(
            0 - m_camera_x,
            row * kTilesize - m_camera_y, 
            m_map_width * kTilesize - m_camera_x, 
            row * kTilesize - m_camera_y,
            {0, 0, 0, 255}
        );
    }
    for (unsigned int col = 1; col < m_map_width; ++col)
    {
        QcE::get_instance()->drawLine(
            col * kTilesize - m_camera_x, 
            0 - m_camera_y,
            col * kTilesize - m_camera_x,
            m_map_height * kTilesize - m_camera_y,
            {0, 0, 0, 255}
        );
    }

    // Draw units
    for (const Unit& unit : m_units)
    {
        unit.Render(m_camera_x, m_camera_y);
    }
    // Render UI
    if (m_selected)
    {
        // Render the menu to the top right of the selected unit
        const uint32_t x = (m_selected->getX() + 1) * kTilesize - m_camera_x;
        uint32_t curr_y = m_selected->getY() * kTilesize - m_camera_y - 20;

        constexpr uint32_t text_padding = 6;

        m_ui_texture->render(x, curr_y, 140, 30);
        static Texture move(getTextTexture("move", "ui30", {40, 40, 50, 255}));
        move.render(x + text_padding, curr_y);
        curr_y += 32;
        m_ui_texture->render(x, curr_y, 140, 30);
        static Texture attack(getTextTexture("attack", "ui30", {40, 40, 50, 255}));
        attack.render(x + text_padding, curr_y);
        curr_y += 32;
        m_ui_texture->render(x, curr_y, 140, 30);
        static Texture inventory(getTextTexture("inventory", "ui30", {40, 40, 50, 255}));
        inventory.render(x + text_padding, curr_y);
    }
}