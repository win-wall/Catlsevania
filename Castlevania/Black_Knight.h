#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "Item.h"
#include "Ground.h"
namespace enemy
{
	class Black_Knight :public core::CGameObject
	{
		float start_x, end_x;
		float destination_x;
		bool IsOntheGround = false;
		bool Simon_in_the_zone = false;
		
	public:
		Black_Knight();
		virtual void Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game, vector<core::LPGAMEOBJECT>* coObject) override;
		virtual void LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations) override;
		void Render() override;
		virtual void GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom) override;
		void Set_destination_x(float x) { this->destination_x = x; }
		void Set_start_end(float start_x, float end_x) { this->start_x = start_x; this->end_x = end_x; }
		void Is_Simon_in_the_target(float sx, float sy,bool x);
		
	};
}

