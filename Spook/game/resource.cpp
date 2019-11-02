#include "resource.hpp"
#include "game.hpp"
#include "inventory.hpp"

Resource::Resource(int x, int y)
    : Interactable(x, y)
    , m_collected_texture(new Texture("res/tree_collected.png"))
    , m_collected(false)
    , m_item_id(ItemType::WOOD)
{

}
Resource::Resource(int id, Texture * tex, Texture * collected, int w, int h, int x, int y)
    : Interactable(tex, w, h, x, y)
    , m_collected_texture(collected)
    , m_collected(false)
    , m_item_id(id)
{

}
Resource::~Resource()
{

}

bool Resource::CanInteract() const
{
    return !m_collected && !game->getInventory().Full();
}

void Resource::Interact()
{
    game->getInventory().AddItem(m_item_id);
    m_collected = true;
    delete m_texture;
    m_texture = m_collected_texture;
}