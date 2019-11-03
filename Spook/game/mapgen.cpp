#include "mapgen.hpp"

// TDOO: Make these adjustable
constexpr int width = 15;
constexpr int height = 15;
constexpr int steps = 330;

constexpr int num_resources = 10;
constexpr int num_AIs = 20;

MapGen::MapData MapGen::Generate()
{
    MapData data;
    data.width = width;
    data.height = height;

    int x = rand() % width;
    int y = rand() % height;

    // GENERATE THE TILES FIRST
    data.m_tiles = std::vector<Tile>(width * height, {0});
    for (int i = 0; i < steps; ++i)
    {
        data.m_tiles[y * width + x] = {1};
        bool success = false;
        while(!success)
        {
            int key = rand() % 4;
            if (key == 0 && x > 0)
            {
                success = true;
                x--;
            }
            if (key == 1 && x < width - 1)
            {
                success = true;
                x++;
            }
            if (key == 2 && y > 0)
            {
                success = true;
                y--;
            }
            if (key == 3 && y < height - 1)
            {
                success = true;
                y++;
            }
        }
    }

    // THEN GENERATE THE PLAYERS
    int spawn_x = rand() % width;
    int spawn_y = rand() % height;
    while(data.m_tiles[spawn_y * width + spawn_x].index == 0 || spawn_x == width - 1 || spawn_y == height - 1)
    {
        spawn_x = rand() % width;
        spawn_y = rand() % height;
    }
    data.m_tiles[spawn_y * width + (spawn_x + 1)] = {1};
    data.m_tiles[(spawn_y + 1) * width + spawn_x] = {1};
    data.m_tiles[(spawn_y + 1) * width + (spawn_x + 1)] = {1};
    {
        Player * player = new Player(new Texture("res/IRA.png"), 64, 128, spawn_x, spawn_y);
        data.m_players.push_back(player);
    }
    {
        Player * player = new Player(spawn_x, spawn_y + 1);
        data.m_players.push_back(player);
    }
    {
        Player * player = new Player(spawn_x + 1, spawn_y);
        data.m_players.push_back(player);
    }
    {
        Player * player = new Player(spawn_x + 1, spawn_y + 1);
        data.m_players.push_back(player);
    }

    // THEN GENERATE RESOURCES
    int num_resources_added = num_resources;
    while (num_resources_added > 0)
    {
        int x = rand() % width;
        int y = rand() % height;
        if (data.m_tiles[y * width + x].index == 0) continue;
        if ((x == spawn_x || x == spawn_x + 1) && (y == spawn_y || y == spawn_y + 1)) continue;
        // Only trees for now
        Resource * res = new Resource(x, y);
        data.m_interactables.push_back(res);
        num_resources_added--;
    }

    // FINALLY GENERATE AIs
    int num_AIs_added = num_AIs;
    while(num_AIs_added > 0)
    {
        int x = rand() % width;
        int y = rand() % height;
        if (data.m_tiles[y * width + x].index == 0) continue;
        if ((x == spawn_x || x == spawn_x + 1) && (y == spawn_y || y == spawn_y + 1)) continue;
        for (Interactable * item : data.m_interactables)
        {
            if (item->getX() == x && item->getY() == y) continue;
        }
        // 50/50 chance for now
        int key = rand() % 2;
        if (key == 0)
        {
            AI * unit = new AI(new Texture("res/pig.png"), 90, 90, x, y);
            unit->SetMaxHealth(2);
            unit->SetItemDrop(ItemType::MEAT);
            data.m_AIs.push_back(unit);
        }
        if (key == 1)
        {
            AI * unit = new AI(new Texture("res/enemy.png"), 64, 128, x, y);
            unit->SetStrategy(AI::Strategy::HOSTILE_DUMB);
            data.m_AIs.push_back(unit);
        }
        num_AIs_added--;
    }

    return data;
}