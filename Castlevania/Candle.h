#pragma once
#include "GameObject.h"
#include "Item.h"
namespace static_object
{
	class Candle : public core::CGameObject
	{
	public:
		Candle();
		virtual void Render();
		virtual void GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom);
		virtual void Update(DWORD dt, vector<core::LPGAMEOBJECT>* Objects, vector<core::LPGAMEOBJECT>* coObject);
		void SetStatus(int Status);
		virtual void LoadResources(core::CTexture * &textures, core::CSprites * &sprites, core::CAnimations * &animations);
	};
}

