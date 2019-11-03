#include "player.hpp"
#include "game.hpp"
#include "interactable.hpp"

Player::Player(int x, int y)
	: Unit(new Texture("res/unit.png"), 64, 128, x, y)
	, m_inputState(InputState::NONE)
	, m_hunger(kMaxHunger)
{

}

Player::Player(Texture * tex, int w, int h, int x, int y)
    : Unit(tex, w, h, x, y)
    , m_inputState(InputState::NONE)
	, m_hunger(kMaxHunger)
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
        Math::Rectangle interact(x, curr_y, 140, 30);
        if (Math::isColliding(mousePos, interact))
        {
            setState(InputState::INTERACT);
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
		if (m_inputState == InputState::INTERACT)
		{
			if (HandleClickInteract(mouse_x, mouse_y, cam_x, cam_y, tileSize)) return true;
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
    static Texture interact(QcE::get_instance()->getTextEngine()->getTexture("interact", "ui30", {40, 40, 50, 255}));
    interact.render(x + text_padding, curr_y);
}

void Player::StartTurn()
{
	Unit::StartTurn();
	m_hunger -= 5;
	if (m_hunger <= 0)
	{
		m_hunger = 0;
		TakeDamage(1);
	}
}

bool Player::HandleClickMove(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize)
{
    if (m_movesLeft <= 0) return false;

	int mouse_tile_x = (mouse_x + cam_x) / tilesize;
	int mouse_tile_y = (mouse_y + cam_y) / tilesize;

	// Make sure mouse clicked within 1 square
	if (std::abs(mouse_tile_x - (int)m_pos_x) + std::abs(mouse_tile_y - (int)m_pos_y) != 1)
	{
		return false;
	}

	if (!game->checkOccupied(mouse_tile_x, mouse_tile_y))
	{
		m_inputState = InputState::NONE;
		m_movesLeft--;
		m_pos_y = mouse_tile_y;
		m_pos_x = mouse_tile_x;
		return true;
	}
	return false;
}

bool Player::HandleClickAttack(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize)
{
    if (m_attacked) return false;

	int mouse_tile_x = (mouse_x + cam_x) / tilesize;
	int mouse_tile_y = (mouse_y + cam_y) / tilesize;

	// TODO: Make sure attack is valid for different attack types
	if (std::abs(mouse_tile_x - (int)m_pos_x) + std::abs(mouse_tile_y - (int)m_pos_y) != 1)
	{
		return false;
	}

	if (Unit * unit = game->getUnitAt(mouse_tile_x, mouse_tile_y))
	{
		m_inputState = InputState::NONE;
		m_attacked = true;
		unit->TakeDamage(1);
		return true;
	}
	return false;
}

bool Player::HandleClickInteract(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize)
{
	int mouse_tile_x = (mouse_x + cam_x) / tilesize;
	int mouse_tile_y = (mouse_y + cam_y) / tilesize;

	// Make sure mouse clicked within 1 square
	if (std::abs(mouse_tile_x - (int) m_pos_x) + std::abs(mouse_tile_y - (int) m_pos_y) != 1)
	{
		return false;
	}

	if (Interactable * interactable = game->getInteractable(mouse_tile_x, mouse_tile_y))
	{
		m_inputState = InputState::NONE;
		interactable->Interact();
		return true;
	}
	return false;
}