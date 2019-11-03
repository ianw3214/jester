#pragma once
class Player;
class AI;
class Interactable;

#include <vector>

//////////////////////////////////////////////////////
struct Tile
{
    int index;
};

class MapGen
{
public:
    struct MapData
    {
        int width;
        int height;

        std::vector<Tile> m_tiles;
        std::vector<Player*> m_players;
        std::vector<AI*> m_AIs;
        std::vector<Interactable*> m_interactables;
    };

    static MapData Generate();
};