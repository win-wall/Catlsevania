#pragma once
#include "GameObject.h"
#include <vector>
namespace core
{
	class Unit
	{
		friend class Grid;
	private:
		Grid* grid;
		LPGAMEOBJECT obj;
		float _x;
		float _y;

		//trỏ cho double linked list
		Unit* _prev;
		Unit* _next;
	public:
		Unit(Grid* grid, LPGAMEOBJECT obj, float _x, float _y);

		void Move(float x, float y);
		LPGAMEOBJECT GetObj() { return this->obj; }
	};
	class Grid
	{
		int map_width;
		int map_height;

		int cell_width;
		int cell_height;

		int nums_col;
		int nums_row;
		vector <vector<Unit*>> cells; //1 cell chứa nhiều unit
	public:
		Grid(int map_width, int map_height, int cell_width, int cell_height);
		~Grid();

		void Add(Unit* unit);
		void Move(Unit* unit, float x, float y);
		void Get(float cam_x, float cam_y, vector<Unit*>& listUnits);

		void Out();
	};
}

