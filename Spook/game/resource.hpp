#pragma once
#include "interactable.hpp"
class GameState;

class Resource : public Interactable
{
public:
    Resource(int x = 0, int y = 0);
    Resource(int id, Texture * tex, Texture * collected, int w, int h, int x = 0, int y = 0);
    ~Resource();

    virtual bool CanInteract() const override;
	virtual void Interact() override;
private:
    Texture * m_collected_texture;

    // Resource data
    bool m_collected;
    // Each collectible only drops one item
    int m_item_id;
};