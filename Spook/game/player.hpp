#pragma once
#include "unit.hpp"

class Player : public Unit
{
public:
    enum class InputState
    {
        NONE,
        MOVE,
        ATTACK,
        INTERACT,
        INVENTORY
    };

    Player(int x = 0, int y = 0);
    Player(Texture * tex, int w, int h, int x = 0, int y = 0);
    ~Player();

    void setState(InputState state) { m_inputState = state; }
    InputState getState() const { return m_inputState; }

    void RenderUI(int cam_x, int cam_y, int tilesize, Texture * base) const;

    bool HandleClick(int mouse_x, int mouse_y, int cam_x, int cam_y, int tileSize);
    void Select();
    void Deselect();

private:
    InputState m_inputState;

    // Helper functions
    bool HandleClickMove(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize);
    bool HandleClickAttack(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize);
    bool HandleClickInteract(int mouse_x, int mouse_y, int cam_x, int cam_y, int tilesize);
};