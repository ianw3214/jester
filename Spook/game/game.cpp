#include "game.hpp"

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
    m_white_overlay = new Texture("res/white_overlay.png");

    // Initialize some units
    {
        Player * player = new Player(0, 0);
        player->SetGameState(this);
        m_units.push_back(player);
        m_players.push_back(player);   
    }
    {
        Player * player = new Player(6, 7);
        player->SetGameState(this);
        m_units.push_back(player);
        m_players.push_back(player);   
    }
    {
        Player * player = new Player(4, 3);
        player->SetGameState(this);
        m_units.push_back(player);
        m_players.push_back(player);   
    }
    {
        Unit * unit = new Unit(new Texture("res/enemy.png"), 64, 128, 2, 2);
        unit->SetGameState(this);
        m_units.push_back(unit);
    }
    {
        Unit * unit = new Unit(new Texture("res/enemy.png"), 64, 128, 3, 3);
        unit->SetGameState(this);
        m_units.push_back(unit);
    }
    {
        Unit * unit = new Unit(new Texture("res/enemy.png"), 64, 128, 4, 4);
        unit->SetGameState(this);
        m_units.push_back(unit);
    }
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
        // Handle inputs for the current selected unit
        if (m_selected)
        {
            if (m_selected->HandleClick(getMouseX(), getMouseY(), m_camera_x, m_camera_y, kTilesize))
            {
                handled = true;
            }
        }
        if (!handled)
        {
            int mouseTileX = (getMouseX() + m_camera_x) / kTilesize;
            int mouseTileY = (getMouseY() + m_camera_y) / kTilesize;
            for (Player * player : m_players)
            {
                if (player->getX() == mouseTileX && player->getY() == mouseTileY)
                {
                    if (m_selected) m_selected->Deselect();
                    m_selected = player;
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

    // Render overlays when needed
    if (m_selected)
    {
        if (m_selected->getState() == Player::InputState::MOVE)
        {
            // Just assume the player can only melee attack for now
            int x = m_selected->getX() * kTilesize;
            int y = m_selected->getY() * kTilesize;
            
            if (!checkOccupied(m_selected->getX() - 1, m_selected->getY()))
            {
                m_white_overlay->render(
                x - kTilesize - m_camera_x,
                y - m_camera_y,
                kTilesize,
                kTilesize
            );
            }
            if (!checkOccupied(m_selected->getX() + 1, m_selected->getY()))
            {
                m_white_overlay->render(
                x + kTilesize - m_camera_x,
                y - m_camera_y,
                kTilesize,
                kTilesize
            );
            }
            if (!checkOccupied(m_selected->getX(), m_selected->getY() - 1))
            {
                m_white_overlay->render(
                x - m_camera_x,
                y - kTilesize - m_camera_y,
                kTilesize,
                kTilesize
            );
            }
            if (!checkOccupied(m_selected->getX(), m_selected->getY() + 1))
            {
                m_white_overlay->render(
                x - m_camera_x,
                y + kTilesize - m_camera_y,
                kTilesize,
                kTilesize
            );
            }
        }
        if (m_selected->getState() == Player::InputState::ATTACK)
        {
            // Just assume the player can only melee attack for now
            int x = m_selected->getX() * kTilesize;
            int y = m_selected->getY() * kTilesize;
            
            if (getUnitAt(m_selected->getX() - 1, m_selected->getY()))
            {
                m_white_overlay->render(
                    x - kTilesize - m_camera_x,
                    y - m_camera_y,
                    kTilesize,
                    kTilesize
                );
            }
            if (getUnitAt(m_selected->getX() + 1, m_selected->getY()))
            {
                m_white_overlay->render(
                    x + kTilesize - m_camera_x,
                    y - m_camera_y,
                    kTilesize,
                    kTilesize
                );   
            }
            if (getUnitAt(m_selected->getX(), m_selected->getY() - 1))
            {
                m_white_overlay->render(
                    x - m_camera_x,
                    y - kTilesize - m_camera_y,
                    kTilesize,
                    kTilesize
                );   
            }
            if (getUnitAt(m_selected->getX(), m_selected->getY() + 1))
            {
                m_white_overlay->render(
                    x - m_camera_x,
                    y + kTilesize - m_camera_y,
                    kTilesize,
                    kTilesize
                );   
            }
        }
        if (m_selected->getState() == Player::InputState::INVENTORY)
        {
            // Do nothing...
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
    for (const Unit * unit : m_units)
    {
        unit->Render(m_camera_x, m_camera_y, kTilesize);
    }
    // Draw unit health
    for (const Unit * unit : m_units)
    {
        unit->RenderHealth(m_camera_x, m_camera_y, kTilesize);
    }
    // Render UI
    if (m_selected && m_selected->getState() == Player::InputState::NONE)
    {
        m_selected->RenderUI(m_camera_x, m_camera_y, kTilesize, m_ui_texture);
    }
}

bool GameState::checkOccupied(unsigned int x, unsigned int y) const
{
    if (x < 0 || x >= m_map_width || y < 0 || y > m_map_height) return true;
    for (const Unit * const unit : m_units)
    {
        if (unit->getX() == x && unit->getY() == y) return true;
    }
    return false;
}

Unit * GameState::getUnitAt(unsigned int x, unsigned int y)
{
    for (Unit * unit : m_units)
    {
        if (unit->getX() == x && unit->getY() == y)
        {
            return unit;
        }
    }
    return nullptr;
}