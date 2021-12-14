#pragma once
#include "GameObject.h"
namespace static_object
{
	class Ground : public core::CGameObject
	{
	public:
		Ground();
		virtual void Render();
		virtual void GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom);
		void SetStatus(int Status);
		virtual void LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations);
	};
}

