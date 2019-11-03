#include "game.hpp"

#include "unit.hpp"
#include "ai.hpp"
#include "player.hpp"
#include "interactable.hpp"
#include "resource.hpp"

#include <algorithm>

GameState::GameState()
    : m_map_width(10)
    , m_map_height(10)
	, m_inventory(this)
	, m_state(State::GAME)
    , m_camera_x(0)
    , m_camera_y(0)
    , m_selected(nullptr)
	, m_turnDelta(0)
    , m_playerTurn(false)
    , m_panning(false)
	, m_craftingIndex(ItemType::WOOD)
	, m_next_level(false)
{
	SDL_GetWindowSize(QcEngine::getWindow(), &m_screenWidth, &m_screenHeight);
	SDL_ShowCursor(0);

    // Initialize textures
    createFont("ui40", "res/Munro.ttf", 30);
    m_tile_texture = new Texture("res/tile.png");
    m_ui_texture = new Texture("res/ui_base.png");
	m_ui_dark = new Texture("res/ui_dark.png");
    m_white_overlay = new Texture("res/white_overlay.png");
    m_end_turn = new Texture("res/endturn.png");
	m_crafting = new Texture("res/crafting.png");
	m_craftingBackground = new Texture("res/crafting_bg.png");
	m_craftingLeft = new Texture("res/crafting_left.png");
	m_craftingRight = new Texture("res/crafting_right.png");
	m_cursor = new Texture("res/cursor.png");

    MapGen::MapData data = MapGen::Generate();
    m_map_width = data.width;
    m_map_height = data.height;
    m_tilemap = std::move(data.m_tiles);

    for (Player * player : data.m_players)
    {
        player->SetGameState(this);
        m_items.push_back(player);
        m_units.push_back(player);
        m_players.push_back(player);
    }
    for (AI * unit : data.m_AIs)
    {
        unit->SetGameState(this);
        m_items.push_back(unit);
        m_units.push_back(unit);
		m_AIs.push_back(unit);
    }
    for (Interactable * item : data.m_interactables)
    {
        item->SetGameRef(this);
        m_items.push_back(item);
        m_interactables.push_back(item);
    }

    // Start the game centered on the players
    Player * player = m_players.front();
    m_camera_x = static_cast<int>(player->getX() * kTilesize) - m_screenWidth / 2;
	m_camera_y = static_cast<int>(player->getY() * kTilesize) - m_screenHeight / 2;


    StartTurn();
}

GameState::~GameState()
{
    for (GridItem * item : m_items)
    {
        delete item;
    }
    delete m_tile_texture;
    delete m_ui_texture;
	delete m_ui_dark;
    delete m_white_overlay;
    delete m_end_turn;
	delete m_crafting;
	delete m_craftingBackground;
	delete m_craftingLeft;
	delete m_craftingRight;
	delete m_cursor;
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
	// If the player died, handle game over
	int health_total = 0;
	for (const Player * player : m_players)
	{
		health_total += player->GetCurrentHealth();
	}
	if (health_total <= 0)
	{
		// TODO: HAndle..
	}
    // Time delaying on enemy turns
    if (!m_playerTurn)
    {
		if (m_turnDelta > (int)kTurnTime)
		{
			if (!DoAIActions())
			{
				StartTurn();
			}
			m_turnDelta = 0;
		}
		else
		{
			m_turnDelta += delta;
		}
    }
    if (leftMousePressed())
    {
        bool handled = false;
        // Handle inputs for the current selected unit
        if (m_selected && m_state == State::GAME)
        {
            if (m_playerTurn && m_selected->HandleClick(getMouseX(), getMouseY(), m_camera_x, m_camera_y, kTilesize))
            {
                handled = true;
            }
			if (m_inventory.HandleClick(getMouseX(), getMouseY(), m_screenWidth, m_screenHeight))
			{
				handled = true;
			}
        }
        if (!handled)
        {
            // Handle UI
            Vec2 mouse(getMouseX(), getMouseY());
			Math::Rectangle craftingButton(0, 400, 160, 70);
			if (Math::isColliding(mouse, craftingButton))
			{
				m_state = State::CRAFTING;
				handled = true;
			}
            Math::Rectangle endButton(0, 600, 180, 80);
            if (m_playerTurn && Math::isColliding(mouse, endButton))
            {
                // End the turn here
                EndTurn();
                handled = true;
            }
            if (!handled)
            {
				if (m_state == State::CRAFTING)
				{
					int x = (m_screenWidth - 560) / 2;
					int y = (m_screenHeight - 120) / 2;

					// Left and right buttons
					Math::Rectangle left(x - 120 - 10, y, 120, 120);
					if (Math::isColliding(mouse, left)) 
					{
						// Find the next valid crafting recipe and render it
						ItemDatabase::Recipe * recipe;
						do {
							m_craftingIndex = static_cast<ItemType>(static_cast<int>(m_craftingIndex) - 1);
							if (m_craftingIndex <= ItemType::NONE)
							{
								m_craftingIndex = static_cast<ItemType>(static_cast<int>(ItemType::COUNT) - 1);
							}
							recipe = &(ItemDatabase::recipes[m_craftingIndex]);
						} while (recipe->item1 == ItemType::NONE && recipe->item2 == ItemType::NONE && recipe->item3 == ItemType::NONE);
						handled = true;
					}
					Math::Rectangle right(x + 560 + 10, y, 120, 120);
					if (Math::isColliding(mouse, right))
					{
						// Find the next valid crafting recipe and render it
						ItemDatabase::Recipe * recipe;
						do {
							m_craftingIndex = static_cast<ItemType>(static_cast<int>(m_craftingIndex) + 1);
							if (m_craftingIndex >= ItemType::COUNT)
							{
								m_craftingIndex = static_cast<ItemType>(0);
							}
							recipe = &(ItemDatabase::recipes[m_craftingIndex]);

						} while (recipe->item1 == ItemType::NONE && recipe->item2 == ItemType::NONE && recipe->item3 == ItemType::NONE);
						handled = true;
					}

					Math::Rectangle craft(x + 440, y + 10, 100, 100);
					if (Math::isColliding(mouse, craft))
					{
						if (m_inventory.HasItemsFor(m_craftingIndex))
						{
							m_inventory.Craft(m_craftingIndex);
						}
						else
						{
							// TODO: User feedback for not enough items
						}
						handled = true;
					}
					// Exit the crafting screen the user clicks somewhere else
					if (!handled)
					{
						Math::Rectangle area(x, y, 220, 52);
						if (!Math::isColliding(mouse, area))
						{
							m_state = State::GAME;
							handled = true;
						}
					}
				}
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
	// State transitions?
	if (m_next_level)
	{
		changeState(std::make_unique<GameState>());
	}
}

void GameState::render()
{
    for (unsigned int y = 0; y < m_map_height; ++y)
    {
        for (unsigned int x = 0; x < m_map_width; ++x)
        {
            const Tile& tile = m_tilemap[y * m_map_height + x];
            if (tile.index == 1)
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
    if (m_playerTurn)
    {
        if (m_selected)
        {
            if (m_selected->getState() == Player::InputState::MOVE && m_selected->getMovesLeft() > 0)
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
            if (m_selected->getState() == Player::InputState::ATTACK && !m_selected->getAttacked())
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
			if (m_selected->getState() == Player::InputState::INTERACT)
			{
				// Player can only interact with neighboor tiles
				int x = m_selected->getX() * kTilesize;
				int y = m_selected->getY() * kTilesize;

				if (getInteractable(m_selected->getX() - 1, m_selected->getY()))
				{
					m_white_overlay->render(
						x - kTilesize - m_camera_x,
						y - m_camera_y,
						kTilesize,
						kTilesize
					);
				}
				if (getInteractable(m_selected->getX() + 1, m_selected->getY()))
				{
					m_white_overlay->render(
						x + kTilesize - m_camera_x,
						y - m_camera_y,
						kTilesize,
						kTilesize
					);
				}
				if (getInteractable(m_selected->getX(), m_selected->getY() - 1))
				{
					m_white_overlay->render(
						x - m_camera_x,
						y - kTilesize - m_camera_y,
						kTilesize,
						kTilesize
					);
				}
				if (getInteractable(m_selected->getX(), m_selected->getY() + 1))
				{
					m_white_overlay->render(
						x - m_camera_x,
						y + kTilesize - m_camera_y,
						kTilesize,
						kTilesize
					);
				}
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

    // Draw items
    std::sort(m_items.begin(), m_items.end(), [](const GridItem * a, const GridItem * b){
        return a->getY() < b->getY();
    });
    for (const GridItem * item : m_items)
    {
		if (item->ShouldRender())
		{
			item->Render(m_camera_x, m_camera_y, kTilesize);
		}
    }
    // Draw unit health
    for (const Unit * unit : m_units)
    {
		if (unit->ShouldRender() && !unit->IsPlayer())
		{
			unit->RenderHealth(m_camera_x, m_camera_y, kTilesize);
		}

    }
    // Render UI
    if (m_playerTurn)
    {
        if (m_selected && m_selected->getState() == Player::InputState::NONE)
        {
            m_selected->RenderUI(m_camera_x, m_camera_y, kTilesize, m_ui_texture, m_ui_dark);
        }
    }
	m_crafting->render(0, 400, 160, 70);
    m_end_turn->render(0, 600, 180, 80);
	m_inventory.Render(m_screenWidth, m_screenHeight);

	renderPlayerPortraits();

	if (m_state == State::CRAFTING)
	{
		int x = (m_screenWidth - 560) / 2;
		int y = (m_screenHeight - 120) / 2;
		m_craftingBackground->render(x, y, 560, 120);

		// Left and right buttons
		m_craftingLeft->render(x - 120 - 10, y, 120, 120);
		m_craftingRight->render(x + 560 + 10, y, 120, 120);

		// Find the next valid crafting recipe and render it
		ItemDatabase::Recipe * recipe = &(ItemDatabase::recipes[m_craftingIndex]);
		while (recipe->item1 == ItemType::NONE && recipe->item2 == ItemType::NONE && recipe->item3 == ItemType::NONE)
		{
			m_craftingIndex = static_cast<ItemType>(static_cast<int>(m_craftingIndex) + 1);
			if (m_craftingIndex >= ItemType::COUNT)
			{
				m_craftingIndex = static_cast<ItemType>(0);
			}
			recipe = &(ItemDatabase::recipes[m_craftingIndex]);
		}
		if (recipe->item1 != ItemType::NONE)
		{
			Texture texture(ItemDatabase::items[recipe->item1].icon);
			texture.render(x + 10, y + 10, 100, 100);
		}
		if (recipe->item2 != ItemType::NONE)
		{
			Texture texture(ItemDatabase::items[recipe->item2].icon);
			texture.render(x + 120, y + 10, 100, 100);
		}
		if (recipe->item3 != ItemType::NONE)
		{
			Texture texture(ItemDatabase::items[recipe->item3].icon);
			texture.render(x + 230, y + 10, 100, 100);
		}
		Texture texture(ItemDatabase::items[m_craftingIndex].icon);
		texture.render(x + 450, y + 10, 100, 100);
	}
	// ALWAYS RENDER THE CURSOR LAST
	m_cursor->render(getMouseX(), getMouseY());
}

bool GameState::checkOccupied(unsigned int x, unsigned int y) const
{
    if (x < 0 || x >= m_map_width || y < 0 || y >= m_map_height) return true;
	if (m_tilemap[y * m_map_width + x].index == 0) return true;
    for (const GridItem * const item : m_items)
    {
        if (item->getX() == x && item->getY() == y && item->Collidable()) return true;
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

Interactable * GameState::getInteractable(unsigned int x, unsigned int y)
{
	for (Interactable * interactable : m_interactables)
	{
		if (interactable->getX() == x && interactable->getY() == y)
		{
			return interactable->CanInteract() ? interactable : nullptr;
		}
	}
	return nullptr;
}

void GameState::nextLevel()
{
	m_next_level = true;
}

void GameState::StartTurn()
{
	for (Player * player : m_players)
	{
		player->StartTurn();
	}
    m_playerTurn = true;
}

void GameState::EndTurn()
{
    m_playerTurn = false;
	// HAVE THE AIs Take their turn
	for (AI * ai : m_AIs)
	{
		ai->MakePlan();
	}
}

bool GameState::DoAIActions()
{
	bool action_taken = false;
	for (AI * ai : m_AIs)
	{
		if (ai->TakeAction()) action_taken = true;
	}
	return action_taken;
}

void GameState::renderPlayerPortraits()
{
	for (unsigned int index = 0; index < m_players.size(); ++index)
	{
		Player * player = m_players[index];
		if (player->GetPortraitTexture())
		{
			player->GetPortraitTexture()->render(index * kPortraitWidth + (index + 1) * kPortraitPadding, 20);
		}
		else 
		{
			static Texture portrait("res/portrait_temp.png");
			portrait.render(index * kPortraitWidth + (index + 1) * kPortraitPadding, 20);
		}

		{	// Draw the health
			static Texture green("res/green.png");
			int w = static_cast<int>(100.f * (static_cast<float>(player->GetCurrentHealth()) / static_cast<float>(player->GetMaxHealth())));
			green.render(index * kPortraitWidth + (index + 1) * kPortraitPadding + 140, 30, w, 20);
		}

		{	// Draw the hunger level
			static Texture yellow("res/yellow.png");
			
			int w = static_cast<int>(100.f * (static_cast<float>(player->GetHunger()) / static_cast<float>(kMaxHunger)));
			yellow.render(index * kPortraitWidth + (index + 1) * kPortraitPadding + 140, 60, w, 20);
		}
	
	}
}