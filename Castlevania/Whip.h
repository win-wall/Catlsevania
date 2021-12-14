#pragma once

#include "GameObject.h"
namespace simon
{
	class Whip: public core::CGameObject
	{
	public:
		Whip();

		virtual void Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game = NULL, vector<core::LPGAMEOBJECT>* coObject = NULL) {}
		virtual void Render() {}
		virtual void GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom);
		void Render(int currentID);
		virtual void LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations) override;
		/////////////////////
		//////////METHODS////
		/////////////////////
		void SetWhipPosition(float simon_position_x,float simon_position_y, bool isStand);
		bool CheckCollisionWhipAndStuff(float obj_left, float obj_top, float obj_right, float obj_bottom);
	};
}



