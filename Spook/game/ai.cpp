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
	// Pick a target and try to move to it
	std::vector<Player*>& players = game->getPlayers();

	int key = rand() % players.size();
	Player * target = players[key];
	// Pick a random square
	int x_offset = 0;
	int y_offset = 0;
	int key2 = rand() % 4;
	if (key2 == 0) x_offset = 1;
	if (key2 == 1) x_offset = -1;
	if (key2 == 2) y_offset = 1;
	if (key2 == 3) y_offset = -1;
	Path path;
	if (findPath(Vec2{ (int) m_pos_x, (int) m_pos_y }, Vec2{ (int) target->getX() + x_offset, (int) target->getY() + y_offset }, path, 10))
	{
		m_pos_x = target->getX() + x_offset;
		m_pos_y = target->getY() + y_offset;
		target->TakeDamage(1);
	} 
	else
	{
		HandlePeacefulStrategy();
	}
}

bool AI::findPath(Vec2 start, Vec2 end, Path & target, int max_steps)
{
	// If the target space is occupied, just FAIL
	if (game->checkOccupied(end.x, end.y))
	{
		return false;
	}
	// if the start and end is the same tile just return an empty path
	if (start.x == end.x && start.y == end.y) {
		target = Path();
		return true;
	} else {
		// setup open and closed list of tiles
		std::vector<Path> open;
		std::vector<Vec2> closed;
		// add initial tile to the open set
		int starting_score = std::abs(start.x - end.x) + std::abs(start.y - end.y);
		open.push_back({ std::vector<Vec2>({ Vec2(start.x, start.y) }), 0, starting_score });
		// while there are paths in the open list, look for the optimal path
		while (open.size() > 0) {
			// find the current path in open with the smallest score
			Path shortest = open[0];
			for (const Path& p : open) {
				int score = p.cost + (std::abs(p.path[p.path.size() - 1].x - end.x)) +
					(std::abs(p.path[p.path.size() - 1].y - end.y));
				if (score < shortest.score) {
					shortest = p;
				}
			}
			// then remove it from open and add it to closed
			closed.push_back(shortest.path[shortest.path.size() - 1]);
			open.erase(std::find(open.begin(), open.end(), shortest));
			// then compute adjacent squares
			Vec2 cur_pos = shortest.path[shortest.path.size() - 1];
			if (!game->checkOccupied(cur_pos.x, cur_pos.y - 1)) {	// above
				Vec2 new_pos = cur_pos;
				new_pos.y--;
				if (pathfindHelper(end, open, closed, new_pos, shortest, target)) return true;
			}
			if (!game->checkOccupied(cur_pos.x + 1, cur_pos.y)) {	// right
				Vec2 new_pos = cur_pos;
				new_pos.x++;
				if (pathfindHelper(end, open, closed, new_pos, shortest, target)) return true;
			}
			if (!game->checkOccupied(cur_pos.x, cur_pos.y + 1)) {	// below
				Vec2 new_pos = cur_pos;
				new_pos.y++;
				if (pathfindHelper(end, open, closed, new_pos, shortest, target)) return true;
			}
			if (!game->checkOccupied(cur_pos.x - 1, cur_pos.y)) {	// left
				Vec2 new_pos = cur_pos;
				new_pos.x--;
				if (pathfindHelper(end, open, closed, new_pos, shortest, target)) return true;
			}
		}
	}

	return false;

}

bool AI::pathfindHelper(Vec2 end, std::vector<Path>& open, std::vector<Vec2>& closed, Vec2 new_pos, const Path& shortest, Path& target)
{
	// check to see if we have our desired path already
	if (new_pos.x == end.x && new_pos.y == end.y) {
		std::vector<Vec2> new_path = shortest.path;
		new_path.push_back(new_pos);
		target = { new_path, shortest.cost + 1 };
		return true;
	}
	// check to make sure new position is not already in closed
	for (const Vec2& v : closed) {
		if (new_pos == v) {
			return false;
		}
	}
	bool found_open = false;
	// check to see if this path is in the open list
	for (Path & p : open) {
		if (new_pos == p.path[p.path.size() - 1]) {
			// compute the score and compare if we found the same one
			int old_cost = p.score;
			int new_cost = shortest.score + 1;
			if (new_cost < old_cost) {
				std::vector<Vec2> new_path = shortest.path;
				new_path.push_back(new_pos);
				int new_score = std::abs(new_pos.x - end.x) + std::abs(new_pos.y - end.y) + shortest.cost + 1;
				p = Path{ new_path, shortest.cost + 1, new_cost };
			}
			found_open = true;
		}
	}
	// finally, add the path directly if we didn't find a duplicate
	if (!found_open) {
		std::vector<Vec2> new_path = shortest.path;
		new_path.push_back(new_pos);
		int new_score = std::abs(new_pos.x - end.x) + std::abs(new_pos.y - end.y) + shortest.cost + 1;
		open.push_back({ new_path, shortest.cost + 1, new_score });
	}
	return false;
}