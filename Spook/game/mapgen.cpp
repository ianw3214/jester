#include "mapgen.hpp"

#include "player.hpp"
#include "ai.hpp"
#include "resource.hpp"
#include "goal.hpp"

// TDOO: Make these adjustable
constexpr int width = 20;
constexpr int height = 20;
constexpr int steps = 400;

constexpr int num_resources = 10;
constexpr int num_AIs = 15;

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
		player->SetPortraitTexture(new Texture("res/portrait_IRA.png"));
		data.m_players.push_back(player);
    }
    {
        Player * player = new Player(new Texture("res/ZAYA.png"), 64, 96, spawn_x, spawn_y + 1);
        player->SetPortraitTexture(new Texture("res/portrait_ZAYA.png"));
        data.m_players.push_back(player);
    }
    {
        Player * player = new Player(new Texture("res/TICA.png"), 80, 80, spawn_x + 1, spawn_y);
        player->SetPortraitTexture(new Texture("res/portrait_TICA.png"));
        data.m_players.push_back(player);
    }
    {
        Player * player = new Player(new Texture("res/BROCK.png"), 130, 140, spawn_x + 1, spawn_y + 1);
        player->SetPortraitTexture(new Texture("res/portrait_BROCK.png"));
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
        bool overlap = false;
        for (Interactable * item : data.m_interactables)
        {
            if (item->getX() == x && item->getY() == y) overlap = true;
        }
        if (overlap) continue;
        // Only trees for now
        Resource * res = new Resource(x, y);
        data.m_interactables.push_back(res);
        num_resources_added--;
    }

    // GENERATE AIs
    int num_AIs_added = num_AIs;
    while(num_AIs_added > 0)
    {
        int x = rand() % width;
        int y = rand() % height;
        if (data.m_tiles[y * width + x].index == 0) continue;
        if ((x == spawn_x || x == spawn_x + 1) && (y == spawn_y || y == spawn_y + 1)) continue;
        bool overlap = false;
        for (Interactable * item : data.m_interactables)
        {
            if (item->getX() == x && item->getY() == y) overlap = true;
        }
        for (AI * ai : data.m_AIs)
        {
            if (ai->getX() == x && ai->getY() == y) overlap = true;
        }
        if (overlap) continue;
        int key = rand() % 2;
        ////////////////////////////////// PIG
        if (key == 0)
        {
            AI * unit = new AI(new Texture("res/pig.png"), 90, 70, x, y);
            unit->SetMaxHealth(2);
            unit->SetItemDrop(ItemType::MEAT);
            data.m_AIs.push_back(unit);
        }
        if (key == 1)
        {
			AnimatedTexture * tex = new AnimatedTexture("res/enemy.png");
			tex->generateAtlas(64, 128);
			tex->addAnimationState(std::make_pair<int, int>(0, 29));
			tex->changeAnimation(0);
            AI * unit = new AI(tex, 64, 128, x, y);
            unit->SetStrategy(AI::Strategy::HOSTILE_DUMB);
            data.m_AIs.push_back(unit);
        }
        num_AIs_added--;
    }

    // GENERATE THE END GOAL
    bool goal_added = false;
    while(!goal_added)
    {
        int x = rand() % width;
        int y = rand() % height;
        if (data.m_tiles[y * width + x].index == 0) continue;
        if ((x == spawn_x || x == spawn_x + 1) && (y == spawn_y || y == spawn_y + 1)) continue;
		bool overlap = false;
        for (Interactable * item : data.m_interactables)
        {
            if (item->getX() == x && item->getY() == y) overlap = true;
        }
        for (AI * ai : data.m_AIs)
        {
            if (ai->getX() == x && ai->getY() == y) overlap = true;
        }
		if (overlap) continue;
        // Interactable * interactable = new Interactable(new Texture("res/stairs.png"), 100, 100, x, y);
        // data.m_interactables.push_back(interactable);
        Goal * goal = new Goal(x, y);
        data.m_interactables.push_back(goal);
        goal_added = true;
    }

    return data;
}