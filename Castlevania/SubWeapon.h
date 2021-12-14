#pragma once
#include "GameObject.h"
#include "Simon.h"
#include "Texture.h"
#include "Utils.h"
#include "Candle.h"
namespace simon
{
	class SubWeapon: public core::CGameObject
	{
	public:
		bool Is_Holy_Water_On_The_Ground = false;
		int Holy_Water_On_The_Ground_Counter = 0;
		SubWeapon();
		~SubWeapon();

		virtual void LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations);
		virtual void Update(DWORD dt, vector<core::LPGAMEOBJECT>* List = NULL, vector<core::LPGAMEOBJECT>* coObject = NULL);
		virtual void Render();

		void SetState(int state);
		virtual void GetBBox(float& left, float& top, float& right, float& bottom);
		void StartHolyWaterEffect() { Is_Holy_Water_On_The_Ground = true; Holy_Water_On_The_Ground_Counter = GetTickCount(); }
	};
}

