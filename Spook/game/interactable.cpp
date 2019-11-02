#include "interactable.hpp"

Interactable::Interactable(int x, int y)
    : GridItem(new Texture("res/tree.png"), 64, 128, x, y)
{

}

Interactable::Interactable(Texture * tex, int w, int h, int x, int y)
    : GridItem(tex, w, h, x, y)
{

}

Interactable::~Interactable()
{

}
