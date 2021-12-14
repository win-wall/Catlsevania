#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "Texture.h"
#include "Ground.h"
namespace static_object
{
	class Item:public core::CGameObject
	{
		DWORD time_disappear;
	public:
		Item();
		virtual void LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations) override;
		void Render() override;
		virtual void Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game, vector<core::LPGAMEOBJECT>* coObject) override;

		void SetItem(int ID_item) { this -> status = ID_item; }

		virtual void GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom) override;
	};
}

