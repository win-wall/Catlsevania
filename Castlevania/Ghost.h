#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "Item.h"
#include "Ground.h"
namespace enemy
{
	class Ghost :public core::CGameObject
	{
		float destination_x;
		bool Simon_in_the_zone = false;
		float sx, sy; //simon x y
		bool state1 = false;
		bool state2 = false;
	public:
		Ghost();
		virtual void Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game, vector<core::LPGAMEOBJECT>* coObject) override;
		virtual void LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations) override;
		void Render() override;
		virtual void GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom) override;
		void Set_destination_x(float x) { this->destination_x = x; }
		void Is_Simon_in_the_target(float sx, float sy,bool x);
		void SetStatus(int status);
	};
}

