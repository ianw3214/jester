#include "goal.hpp"
#include "game.hpp"

Goal::Goal(int x, int y)
    : Interactable(new Texture("res/stairs.png"), 100, 100, x, y)
{

}

void Goal::Interact() 
{
    game->nextLevel();
}