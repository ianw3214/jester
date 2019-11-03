#pragma once
#include "unit.hpp"

#include <vector>

constexpr int kMaxMoveTries = 3;

////////////////////////////////////////////////////////////////////////
// struct to keep track of path when pathfinding
struct Path {
	std::vector<Vec2> path;
	int cost;
	int score;

	void operator=(const Path& other) {
		path = other.path;
		cost = other.cost;
		score = other.score;
	}
};

////////////////////////////////////////////////////////////////////////
class AI : public Unit
{
public:
	enum class Strategy
	{
		PEACEFUL,
		HOSTILE_DUMB
	};

    AI(int x = 0, int y = 0);
    AI(Texture * tex, int w, int h, int x = 0, int y = 0);
    ~AI();

	void SetStrategy(Strategy strategy) { m_strategy = strategy; }

	void MakePlan();
	bool TakeAction();

private:
	Strategy m_strategy;

	// Helper functions
	// TODO: NEED PATHFINDING ALGORITHMS
	void HandlePeacefulStrategy();
	void HandleHostileDumbStrategy();

	bool findPath(Vec2 start, Vec2 end, Path & target, int max_steps);
	bool pathfindHelper(Vec2 end, std::vector<Path>& open, std::vector<Vec2>& closed, Vec2 new_pos, const Path& shortest, Path& target);

	// Data of the plan
	Path m_path;
	int m_path_index;
	int m_num_move_tries;
	int m_player_target;
};

// equality operator for paths to compare if 2 paths are the same
inline bool operator==(const Path& p1, const Path& p2) {
	return p1.path == p2.path && p1.score == p2.score && p1.cost == p2.cost;
}

// temporary equality operator for vec2
// TODO: implement in engine
inline bool operator== (const Vec2& v1, const Vec2& v2) {
	return v1.x == v2.x && v1.y == v2.y;
}