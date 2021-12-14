#pragma once

#include "d3dx9.h"
#include "Utils.h"
#include "Texture.h"
#include "Sprite.h"
#include "Simon.h"
#include "SceneManager.h"

#include <vector>
#include <string>

using namespace  std;
namespace HUD
{
	class HUD
	{
	private:
		core::SceneManager* scenes;
		core::CGame* game;
		simon::Simon* simons;

		ID3DXFont* font;
		string information;

		vector<core::LPSPRITE>playerHp;
		vector<core::LPSPRITE>enemyHP;
		vector<core::LPSPRITE>loseHP;

		core::CSprite* subWeaponBox;
		vector<core::LPSPRITE> subWeaponList;
		
		int Score_Simon;
		int Time;
		int Scene;
		int Subweapon_Simon;
		int Mana_Simon;
		int Life_Simon;
		int HP_Simon;
		int Item;
		int BossHP;

	public:
		HUD(core::SceneManager* scenes, core::CGame* game);
		~HUD();

		void Init();
		void Update(DWORD dt);
		void Render();
	};
}

