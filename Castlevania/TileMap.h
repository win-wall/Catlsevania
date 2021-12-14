#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Texture.h"
#include "Sprite.h"
#include "Utils.h"
namespace core
{
	class TileMap
	{
		int Map_Width; //chiều dài của map
		int Map_Height; //chiều cao của map

		int Map_col;// số cột của map được chia ra
		int Map_row;//số hàng của map được chia ra

		int Tile_Width;//chiều dài của từng tile
		int Tile_Height;// chiều rông của từng tile

		LPCWSTR Texture_FilePath;
		LPCWSTR FilePath_Read;

		int ID_Map;
	public:
		vector<vector<int>>map;
		
		
		CSprites* sprites;

		
		void Draw(int camera_x, int camera_y);
		TileMap(int ID_Map,LPCWSTR FilePath_Read, LPCWSTR Texture_FilePath,int Map_Width,int Map_Height,int Tile_Width, int Tile_Height);
		int Get_Map_Width() { return Map_Width; }
		
		void LoadResource();
		void Load_Data();
		
	};
	typedef TileMap* LPTILEMAP;

	class TileMaps
	{
		static TileMaps* _instance;
		unordered_map<int, LPTILEMAP> tilemaps;
	public:
		void Add(int ID_Map, LPCWSTR FilePath_Read, LPCWSTR Texture_FilePath, int Map_Width, int Map_Height, int Tile_Width, int Tile_Height);
		LPTILEMAP Get(int ID_Map) { return tilemaps[ID_Map]; }

		static TileMaps* GetInstance();
	};
}

