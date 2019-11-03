#include "ai.hpp"
#include "game.hpp"
#include "player.hpp"

AI::AI(int x, int y)
    : Unit(x, y)
	, m_strategy(Strategy::PEACEFUL)
{

}

AI::AI(Texture * tex, int w, int h, int x, int y)
    : Unit(tex, w, h, x, y)
	, m_strategy(Strategy::PEACEFUL)
{

}

AI::~AI()
{

}

void AI::DoMoves()
{
	switch (m_strategy)
	{
	case Strategy::PEACEFUL: {
		HandlePeacefulStrategy();
	} break;
	case Strategy::HOSTILE_DUMB: {
		HandleHostileDumbStrategy();
	} break;
	default: {
		// SHOULD NEVER HAPPEN D:
	} break;
	}
}

void AI::HandlePeacefulStrategy()
{
	// TODO: Get from attributes
	constexpr int dexterity = 5;

	int tries = 5;
	while (tries > 0)
	{
		// This isn't very accurate but who the fuck cares lmao
		int x_offset = rand() % dexterity - dexterity / 2;
		int y_offset = rand() % dexterity - dexterity / 2;
		if (!game->checkOccupied(m_pos_x + x_offset, m_pos_y + y_offset))
		{
			m_pos_x = m_pos_x + x_offset;
			m_pos_y = m_pos_y + y_offset;
			return;
		}
		tries--;
	}
}

void AI::HandleHostileDumbStrategy()
{
	// TODO: Get from attributes
	constexpr int dexterity = 5;

	int tries = 5;
	while (tries > 0)
	{
		// This isn't very accurate but who the fuck cares lmao
		int x_offset = rand() % dexterity - dexterity / 2;
		int y_offset = rand() % dexterity - dexterity / 2;
		if (!game->checkOccupied(m_pos_x + x_offset, m_pos_y + y_offset))
		{
			m_pos_x = m_pos_x + x_offset;
			m_pos_y = m_pos_y + y_offset;
			return;
		}
		tries--;
	}
	// Pick a target and try to move to it
	std::vector<Player*>& players = game->getPlayers();

	int key = rand() % players.size();
	Player * target = players[key];
}