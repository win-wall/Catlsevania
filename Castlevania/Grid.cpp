#include "Grid.h"
namespace core
{
	Unit::Unit(Grid* grid, LPGAMEOBJECT obj, float _x, float _y)
	{
		this->grid = grid;
		this->obj = obj;
		this->_x = _x;
		this->_y = _y;

		this->_prev = NULL;
		this->_next = NULL;

		grid->Add(this);
	}
	void Unit::Move(float x, float y)
	{
		grid->Move(this, x, y);
	}
	Grid::Grid(int map_width, int map_height, int cell_width, int cell_height)
	{
		this->map_width = map_width;
		this->map_height = map_height;

		this->cell_width = cell_width;
		this->cell_height = cell_height;

		nums_col = map_width / cell_width;
		nums_row = map_height / cell_height;
		cells.resize(nums_row);
		for(int i=0;i<nums_row;i++)
		{
			cells[i].resize(nums_col);
		}
		//clear
		for(int i=0;i<nums_row;i++)
		{
			for(int j=0;j<nums_col;j++)
			{
				cells[i][j] = NULL;
			}
		}
	}
	Grid::~Grid()
	{
		
	}
	void Grid::Add(Unit* unit)
	{
		int row = (int)(unit->_y / cell_height);
		int col = (int)(unit->_x / cell_width);

		//thêm vào đầu cell
		unit->_prev = NULL;
		unit->_next = cells[row][col];
		cells[row][col] = unit;
		if(unit->_next!=NULL)
		{
			unit->_next->_prev = unit;
		}
	}
	void Grid::Move(Unit* unit, float x, float y)
	{
		//lấy thông số cell cũ
		int old_row = (int)(unit->_y / cell_height);
		int old_col = (int)(unit->_x / cell_width);

		//lấy thông số cell mới
		int new_row = int(y / cell_height);
		int new_col = int(x / cell_width);

		//cập nhật toạ độ mới
		unit->_x = x;
		unit->_y = y;

		//nếu obj chưa ra khỏi cell
		if(old_row==new_row&&old_col==new_col)
		{
			return;
		}
		//nếu obj ra khỏi cell
		// huỷ liên kết với cell cũ
		if(unit->_prev!=NULL)
		{
			unit->_prev->_next = unit->_next;
		}
		if(unit->_next!=NULL)
		{
			unit->_next->_prev = unit->_prev;
		}
		if(cells[old_row][old_col]==unit)
		{
			cells[old_row][old_col] = unit->_next;
		}

		//thêm vào cell mới
		Add(unit);
	}
	void Grid::Get(float cam_x, float cam_y, vector<Unit*>& listUnits)
	{
		int start_col = (int)(cam_x / cell_width);
		int end_col = (int)((cam_x+SCREEN_WIDTH) / cell_height);
		if(end_col != nums_col)
		{
			end_col += 1;
		}
		if(start_col!=0)
		{
			start_col += -1;
		}
		for(int i=0;i<nums_row;i++)
		{
			for(int j=start_col;j<end_col;j++)
			{
				Unit* unit = cells[i][j];
				while(unit!=NULL)
				{
					if(unit->GetObj()->Is_Enable==true)
					{
						listUnits.push_back(unit);
					}
					unit = unit->_next;
				}
			}
		}
	}
	void Grid::Out()
	{
		
	}

}