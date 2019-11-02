#include "player.hpp"
#include "game.hpp"

Player::Player(int x, int y)
    : Unit(x, y)
    , m_inputState(InputState::NONE)
{

}

Player::Player(Texture * tex, int w, int h, int x, int y)
    : Unit(tex, w, h, x, y)
    , m_inputState(InputState::NONE)
{

}

Player::~Player()
{

}

bool Player::HandleClick(int mouse_x, int mouse_y, int cam_x, int cam_y, int tileSize)
{
    // UI - mirrored from render code
    // Render the menu to the top right of the selected unit
    const uint32_t x = (m_pos_x + 1) * tileSize - cam_x;
    uint32_t curr_y = m_pos_y * tileSize - cam_y - 20;

    if(m_inputState == InputState::NONE)
    {
        Vec2 mousePos(mouse_x, mouse_y);
        Math::Rectangle move(x, curr_y, 140, 30);
        if (Math::isColliding(mousePos, move))
        {
            setState(InputState::MOVE);
            return true;
        }
        curr_y += 32;
        Math::Rectangle attack(x, curr_y, 140, 30);
        if (Math::isColliding(mousePos, attack))
        {
            setState(InputState::ATTACK);
            return true;
        }
        curr_y += 32;
        Math::Rectangle inventory(x, curr_y, 140, 30);
        if (Math::isColliding(mousePos, inventory))
        {
            setState(InputState::INVENTORY);
            return true;
        }
    }
    else
    {
        // Update based on current state
        if (m_inputState == InputState::MOVE)
        {
            if (HandleClickMove(mouse_x, mouse_y, cam_x, cam_y, tileSize)) return true;   
        }
        if (m_inputState == InputState::ATTACK)
        {
            if (HandleClickAttack(mouse_x, mouse_y, cam_x, cam_y, tileSize)) return true;
        }
    }
    return false;
}

void Player::Select()
{
    m_texture->setBlendMode(SDL_BLENDMODE_ADD);
    m_texture->setColourModulation({255, 255, 255});
}
void Player::Deselect()
{
    m_texture->setBlendMode(SDL_BLENDMODE_BLEND);
    m_texture->setColourModulation({255, 255, 255});
    m_inputState = InputState::NONE;
}

void Player::RenderUI(int cam_x, int cam_y, int tilesize, Texture * base) const
{
    // Render the menu to the top right of the selected unit
    const uint32_t x = (m_pos_x + 1) * tilesize - cam_x;
    uint32_t curr_y = m_pos_y * tilesize - cam_y - 20;

    constexpr uint32_t text_padding = 6;

    base->render(x, curr_y, 140, 30);
    static Texture move(QcE::get_instance()->getTextEngine()->getTexture("move", "ui30", {40, 40, 50, 255}));
    move.render(x + text_padding, curr_y);
    curr_y += 32;
    base->render(x, curr_y, 140, 30);
    static Texture attack(QcE::get_instance()->getTextEngine()->getTexture("attack", "ui30", {40, 40, 50, 255}));
    attack.render(x + text_padding, curr_y);
    curr_y += 32;
    base->render(x, curr_y, 140, 30);
    static Texture inventory(QcE::get_instance()->getTextEngine()->getTexture("inventory", "ui30", {40, 40, 50, 255}));
    inventory.render(x + text_padding, curr_y);
}

bool Player::HandleClickMove(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize)
{
    if (m_movesLeft <= 0) return false;

    int x = m_pos_x * tilesize;
    int y = m_pos_y * tilesize;

    Vec2 mousePos(mouse_x, mouse_y);

    Math::Rectangle left(x - tilesize - cam_x, y - cam_y, tilesize, tilesize);
    if (!game->checkOccupied(m_pos_x - 1, m_pos_y) && Math::isColliding(mousePos, left))
    {
        m_inputState = InputState::NONE;
        m_movesLeft--;
        m_pos_x--;
        return true;
    }
    Math::Rectangle right(x + tilesize - cam_x, y - cam_y, tilesize, tilesize);
    if (!game->checkOccupied(m_pos_x + 1, m_pos_y) && Math::isColliding(mousePos, right))
    {
        m_inputState = InputState::NONE;
        m_movesLeft--;
        m_pos_x++;
        return true;
    }
    Math::Rectangle up(x - cam_x, y - tilesize - cam_y, tilesize, tilesize);
    if (!game->checkOccupied(m_pos_x, m_pos_y - 1) && Math::isColliding(mousePos, up))
    {
        m_inputState = InputState::NONE;
        m_movesLeft--;
        m_pos_y--;
        return true;
    }
    Math::Rectangle down(x- cam_x, y + tilesize - cam_y, tilesize, tilesize);
    if (!game->checkOccupied(m_pos_x, m_pos_y + 1) && Math::isColliding(mousePos, down))
    {
        m_inputState = InputState::NONE;
        m_movesLeft--;
        m_pos_y++;
        return true;
    }
    return false;
}

bool Player::HandleClickAttack(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize)
{
    if (m_attacked) return false;

    int x = m_pos_x * tilesize;
    int y = m_pos_y * tilesize;

    Vec2 mousePos(mouse_x, mouse_y);

    Math::Rectangle left(x - tilesize - cam_x, y - cam_y, tilesize, tilesize);
    Unit * unit_l = game->getUnitAt(m_pos_x - 1, m_pos_y);
    if (unit_l && Math::isColliding(mousePos, left))
    {
        m_inputState = InputState::NONE;
        m_attacked = true;
        unit_l->TakeDamage(1);
        return true;
    }
    Math::Rectangle right(x + tilesize - cam_x, y - cam_y, tilesize, tilesize);
    Unit * unit_r = game->getUnitAt(m_pos_x + 1, m_pos_y);
    if (unit_r && Math::isColliding(mousePos, right))
    {
        m_inputState = InputState::NONE;
        m_attacked = true;
        unit_r->TakeDamage(1);
        return true;
    }
    Math::Rectangle up(x - cam_x, y - tilesize - cam_y, tilesize, tilesize);
    Unit * unit_u = game->getUnitAt(m_pos_x, m_pos_y - 1);
    if (unit_u && Math::isColliding(mousePos, up))
    {
        m_inputState = InputState::NONE;
        m_attacked = true;
        unit_u->TakeDamage(1);
        return true;
    }
    Math::Rectangle down(x - cam_x, y + tilesize - cam_y, tilesize, tilesize);
    Unit * unit_d = game->getUnitAt(m_pos_x, m_pos_y + 1);
    if (unit_d && Math::isColliding(mousePos, down))
    {
        m_inputState = InputState::NONE;
        m_attacked = true;
        unit_d->TakeDamage(1);
        return true;
    }
    return false;
}