#include "resource.hpp"
#include "game.hpp"

Resource::Resource(int x, int y)
    : Interactable(x, y)
    , m_collected_texture(new Texture("res/tree_collected.png"))
    , m_collected(false)
{

}
Resource::Resource(Texture * tex, Texture * collected, int w, int h, int x, int y)
    : Interactable(tex, w, h, x, y)
    , m_collected_texture(collected)
    , m_collected(false)
{

}
Resource::~Resource()
{

}

bool Resource::CanInteract() const
{
    return !m_collected;
}

void Resource::Interact()
{
    m_collected = true;
    delete m_texture;
    m_texture = m_collected_texture;
}