#pragma once
#include "interactable.hpp"
class GameState;

class Resource : public Interactable
{
public:
    Resource(int x = 0, int y = 0);
    Resource(Texture * tex, Texture * collected, int w, int h, int x = 0, int y = 0);
    ~Resource();

    void SetGameRef(GameState * g) { game = g; }

    virtual bool CanInteract() const override;
	virtual void Interact() override;
private:
    Texture * m_collected_texture;

    // Resource data
    bool m_collected;

    GameState * game;
};