#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>


#include "Game.h"

namespace core
{
	class CSprite
	{
		int SpriteID;
		int left, top, right, bottom;
		LPDIRECT3DTEXTURE9 texture;
	public:
		CSprite(int SpriteID, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
		void Draw(int RENDER_CAM, int nx, float x, float y, int alpha = 255);
	};
	typedef CSprite* LPSPRITE;
	class CSprites
	{
		static CSprites* _instance;
		unordered_map<int, LPSPRITE> sprites;
	public:
		void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
		LPSPRITE Get(int id) { return sprites[id]; }

		static CSprites* GetInstance();
	};
}
