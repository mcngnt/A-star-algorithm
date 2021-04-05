#include "pathfinding.h"

Pos operator+(const Pos& a, Pos b)
{
	return Pos(a.x + b.x, a.y + b.y);
}

bool operator==(const Pos& a, Pos b)
{
	return (a.x == b.x) && (a.y == b.y);
}

bool operator!=(const Pos& a, Pos b)
{
	return !((a.x == b.x) && (a.y == b.y));
}


bool is_in_set(std::vector<Pos> set, Pos pos)
{
	if(std::find(set.begin(), set.end(), pos) != set.end())
	{
	    return true;
	}
	return false;
}
bool is_valid_pos(Pos p)
{
	return (p.x >= 0) && (p.y >= 0) && (p.x < GRID_W) && (p.y < GRID_H);
}

void delete_in_set(std::vector<Pos>* set, Pos pos)
{
	for (unsigned i = 0; i < set->size(); ++i)
	{
		if (set->at(i) == pos)
		{
			set->erase(set->begin() + i);
			return;
		}
	}
}

void print_pos(Pos p)
{
	printf("(%d %d)\n", p.x, p.y);
}


void Pathfinder::generate_wall_map()
{
	for (int x = 0; x < GRID_W; ++x)
	{
		for (int y = 0; y < GRID_H; ++y)
		{
			wallMap[x][y] = false;
			if (rand_float(false) < 0.2 && Pos(x,y) != startPos && Pos(x,y) != endPos)
			{
				wallMap[x][y] = true;
			}
		}
	}
}

void Pathfinder::reset_map()
{
	for (int x = 0; x < GRID_W; ++x)
	{
		for (int y = 0; y < GRID_H; ++y)
		{
			Cell c;
			c.pos = Pos(x, y);
			map[x][y] = c;
		}
	}
}

float Pathfinder::calculate_h_cost(Pos current)
{
	return (current.x-endPos.x)*(current.x-endPos.x) + (current.y-endPos.y)*(current.y-endPos.y);
}

bool Pathfinder::update_neighbour(Pos p)
{
	Pos neighbourMove[8] = {Pos(-1,0), Pos(-1,-1), Pos(0,1), Pos(1, 1), Pos(1, 0), Pos(1, -1), Pos(0,-1), Pos(-1, -1)};
	Cell* parentCell = &map[p.x][p.y];
	closeSet.push_back(p);
	delete_in_set(&openSet, p);
	for (int i = 0; i < 8; ++i)
	{
		Pos currentPos = p + neighbourMove[i];
		if (!is_valid_pos(currentPos) or is_in_set(closeSet, currentPos))
		{
			continue;
		}
		Cell* currentCell = &map[currentPos.x][currentPos.y];

		if (currentPos == endPos)
		{
			return true;
		}

		if (wallMap[currentPos.x][currentPos.y])
		{
			continue;
		}

		float possibleGCost = parentCell->gCost + (i%2) * COST_DIAG + !(i%2) * COST_STRAIGHT;

		if (!is_in_set(openSet, currentPos))
		{
			currentCell->hCost = calculate_h_cost(currentPos);
			currentCell->parent = parentCell;
			currentCell->gCost = possibleGCost;
			openSet.push_back(currentPos);
		}
		else
		{
			if (currentCell->gCost > possibleGCost)
			{
				currentCell->gCost = possibleGCost;
				currentCell->parent = parentCell;
			}
		}
		currentCell->fCost = currentCell->gCost + currentCell->hCost;
	}

	return false;
}

std::vector<Pos> Pathfinder::find_path()
{
	reset_map();
	openSet.clear();
	closeSet.clear();
	openSet.push_back(startPos);
	Cell* currentCell = &map[startPos.x][startPos.y];
	while (true)
	{
		if (openSet.size() == 0)
		{
			return {startPos};
		}
	    if (update_neighbour(currentCell->pos))
	    {
	    	break;
	    }

	    float minFCost = std::numeric_limits<float>::infinity();
	    Cell* minFCostCell = NULL;
	    for (unsigned i = 0; i < openSet.size(); ++i)
	    {
	    	Cell* testedCell = &map[openSet[i].x][openSet[i].y];
	    	if (testedCell->fCost < minFCost)
	    	{
	    		minFCost = testedCell->fCost;
	    		minFCostCell = testedCell;
	    	}
	    }
	    currentCell = minFCostCell;
	}
	std::vector<Pos> final;
	int count = 0;
	while (true)
	{
		count++;
		if (count > GRID_W*GRID_H)
		{
			break;
		}
	    if (currentCell->pos == startPos)
	    {
	    	break;
	    }
	    final.push_back(currentCell->pos);
	    currentCell = currentCell->parent;
	}

	std::reverse(final.begin(),final.end());

	return final;
}