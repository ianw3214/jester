#pragma once
#include "unit.hpp"

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

	void DoMoves();

private:
	Strategy m_strategy;

	// Helper functions
	// TODO: NEED PATHFINDING ALGORITHMS
	void HandlePeacefulStrategy();
	void HandleHostileDumbStrategy();
};