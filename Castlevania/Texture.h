#pragma once

#include <Windows.h>
#include <unordered_map>

#include <d3d9.h>
#include <d3dx9.h>

#include "Utils.h"
#include "Game.h"
namespace core
{
	class CTexture
	{
		static CTexture* _instance;
		unordered_map<int, LPDIRECT3DTEXTURE9> textures;

	public:
		void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
		LPDIRECT3DTEXTURE9 Get(int id) { return textures[id]; }

		static CTexture* GetInstance();
	};
}

