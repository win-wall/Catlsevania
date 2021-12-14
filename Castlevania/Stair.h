#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "Texture.h"

namespace static_object
{
	class Stair :public core::CGameObject
	{
	public:
		Stair();
		~Stair();

		virtual void LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations) override;
		virtual void Render() override;
		virtual void GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom) override;
	};
}


