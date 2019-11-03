#pragma once
#include "unit.hpp"

constexpr int kMaxHunger = 100;
class Player : public Unit
{
public:
    enum class InputState
    {
        NONE,
        MOVE,
        ATTACK,
        INTERACT,
    };

    Player(int x = 0, int y = 0);
    Player(Texture * tex, int w, int h, int x = 0, int y = 0);
    ~Player();

    void setState(InputState state) { m_inputState = state; }
    InputState getState() const { return m_inputState; }

	int GetHunger() const { return m_hunger; }

    void RenderUI(int cam_x, int cam_y, int tilesize, Texture * base) const;

	virtual void StartTurn() override;

    bool HandleClick(int mouse_x, int mouse_y, int cam_x, int cam_y, int tileSize);
    void Select();
    void Deselect();

private:
    InputState m_inputState;

	// Unit properties that only the players have
	int m_hunger;

    // Helper functions
    bool HandleClickMove(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize);
    bool HandleClickAttack(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize);
    bool HandleClickInteract(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize);
};