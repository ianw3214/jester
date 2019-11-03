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

	void SetPortraitTexture(Texture * tex) { m_portrait_texture = tex; }
	Texture * GetPortraitTexture() { return m_portrait_texture; }

	int GetHunger() const { return m_hunger; }
	void AddHunger(int points);

    void RenderUI(int cam_x, int cam_y, int tilesize, Texture * base, Texture * dark) const;

	virtual void StartTurn() override;

    virtual bool IsPlayer() const override { return true; }

    bool HandleClick(int mouse_x, int mouse_y, int cam_x, int cam_y, int tileSize);
    void Select();
    void Deselect();

private:
    InputState m_inputState;

	// Unit properties that only the players have
	int m_hunger;

	Texture * m_portrait_texture;

    // Helper functions
    bool HandleClickMove(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize);
    bool HandleClickAttack(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize);
    bool HandleClickInteract(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize);
};