#pragma once
#include "interactable.hpp"

class Goal : public Interactable
{
public:
    Goal(int x = 0, int y = 0);

	virtual void Interact() override;
};