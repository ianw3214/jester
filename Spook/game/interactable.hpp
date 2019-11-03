#pragma once
#include "griditem.hpp"
class GameState;

class Interactable : public GridItem
{
public:
    Interactable(int x = 0, int y = 0);
    Interactable(Texture * tex, int w, int h, int x = 0, int y = 0);
    ~Interactable();

    void SetGameRef(GameState * g) { game = g; }

	virtual bool CanInteract() const { return true; }
	
	virtual void Interact() {}

protected:
    GameState * game;
};