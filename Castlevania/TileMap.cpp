#include "TileMap.h"
namespace core
{
	TileMap::TileMap(int ID_Map, LPCWSTR FilePath_Read, LPCWSTR Texture_FilePath, int Map_Width, int Map_Height, int Tile_Width, int Tile_Height)
	{
		this->ID_Map = ID_Map;
		this->FilePath_Read = FilePath_Read;
		this->Texture_FilePath = Texture_FilePath;

		sprites = CSprites::GetInstance();

		this->Map_Width = Map_Width;
		this->Map_Height = Map_Height;

		this->Tile_Width = Tile_Width;
		this->Tile_Height = Tile_Height;

		Map_row = Map_Height / Tile_Height;
		Map_col = Map_Width / Tile_Width;

		LoadResource();
		Load_Data();
		
	}
	void TileMap::LoadResource()
	{
		CTexture* texture = CTexture::GetInstance();
		texture->Add(ID_Map, Texture_FilePath, D3DCOLOR_XRGB(255, 255, 255));


		LPDIRECT3DTEXTURE9 texTileMap = texture->Get(ID_Map);

		//lấy thông tin texture(image)
		D3DXIMAGE_INFO imageInfo;
		HRESULT hr = D3DXGetImageInfoFromFile(Texture_FilePath, &imageInfo);

		//Lấy thông tin có bao nhiêu cột và hàng trong image
		int howmuch_row = imageInfo.Height / Tile_Height;
		int howmuch_col = imageInfo.Width / Tile_Width;
		if(hr)
		{
			DebugOut(L"[ERROR]	Can't get image Info from texture Tilemap");
			return;
		}

		int ID_Sprite = 1;
		for(int i=0; i<howmuch_row;i++)
		{
			for(int j=0; j<howmuch_col;j++)
			{
				sprites->Add(1000*ID_Map+ID_Sprite, Tile_Width * j, Tile_Height * i, Tile_Width * (j + 1), Tile_Height * (i + 1), texTileMap);
				ID_Sprite += 1;
			}
		}
	}
	void TileMap::Load_Data()
	{
		fstream map_reader;
		map_reader.open(FilePath_Read, ios::in);

		if(!map_reader)
		{
			DebugOut(L"[ERROR]	CLASS TILEMAP:: can't not open file map data");
			map_reader.close();
			return;
		}
		string line;
		while(!map_reader.eof())
		{
			getline(map_reader, line);

			vector<int> number_In_current_Line;
			stringstream ss(line);
			int n;
			while(ss>>n)
			{
				number_In_current_Line.push_back(n);
			}
			map.push_back(number_In_current_Line);
		}
		map_reader.close();
	}
	
	void TileMap::Draw(int camera_x, int camera_y)
	{
		int col_draw = (int)camera_x / 32;
		int col_end_draw;
		if (Map_Width > 16*32)
		{
			col_end_draw = (int)(camera_x + SCREEN_WIDTH) / 32+1;
		}
		else
		{
			col_end_draw = (int)(camera_x + SCREEN_WIDTH) / 32;
		}
		for (int i = 0; i < Map_row; i++)
		{
			for (int j = col_draw; j < col_end_draw; j++)
			{
				float x = Tile_Width * (j - col_draw) + camera_x - (int)camera_x % 32;
				float y = Tile_Height * i +64;
				sprites->Get(1000*ID_Map+map[i][j])->Draw(1, -1, x, y);
			}
		}
	}

	TileMaps* TileMaps::_instance = NULL;
	void TileMaps::Add(int ID_Map, LPCWSTR FilePath_Read, LPCWSTR Texture_FilePath, int Map_Width, int Map_Height, int Tile_Width, int Tile_Height)
	{
		LPTILEMAP tilemap = new TileMap(ID_Map, FilePath_Read, Texture_FilePath, Map_Width, Map_Height, Tile_Width, Tile_Height);
		tilemaps[ID_Map] = tilemap;
	}
	TileMaps* TileMaps::GetInstance()
	{
		if (_instance == NULL) _instance = new TileMaps();
		return _instance;
	}


}