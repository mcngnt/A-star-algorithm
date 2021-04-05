#include "utils.h"
#include <vector>
#include <algorithm>
#include <limits>

#define GRID_W 150
#define GRID_H 100
#define COST_DIAG 1.41
#define COST_STRAIGHT 1

struct Pos
{
	int x;
	int y;
	Pos(int x_ = 0, int y_ = 0)
	{
		x = x_;
		y = y_;
	}
};

Pos operator+(const Pos&, Pos);
bool operator==(const Pos&, Pos);
bool operator!=(const Pos&, Pos);
bool is_in_set(std::vector<Pos>, Pos);
void delete_in_set(std::vector<Pos>*, Pos);
void print_pos(Pos);
bool is_valid_pos(Pos);

struct Cell
{
	float hCost;
	float gCost;
	float fCost;
	Cell* parent;
	Pos pos;

	Cell()
	{
		hCost = 0.f;
		gCost = 0.f;
		fCost = 0.f;
		parent = NULL;
		pos = Pos(-1,-1);
	}
};

class Pathfinder
{
public:
	void generate_wall_map();
	void reset_map();
	float calculate_h_cost(Pos current);
	bool update_neighbour(Pos p);
	std::vector<Pos> find_path();

	std::vector<Pos> openSet;
	std::vector<Pos> closeSet;
	bool wallMap[GRID_W][GRID_H];
	Pos startPos;
	Pos endPos;

	Cell map[GRID_W][GRID_H];
};