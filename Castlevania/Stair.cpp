#include "Stair.h"
#include <fstream>
#include <iostream>
#include <sstream>
namespace static_object
{
	Stair::Stair()
	{
		Add_Animation_into_state(STAIR_LEFT_UP_ANIMATION);
		Add_Animation_into_state(STAIR_RIGHT_DOWN_ANIMATION);
	}
	Stair::~Stair()
	{
		
	}

	void Stair::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURE_STAIR, L"Textures\\Stair.png", transparentColor);
		LPDIRECT3DTEXTURE9 texStair = textures->Get(ID_TEXTURE_STAIR);
		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Stair.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom, id_tex;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom >> id_tex)
		{
			sprites->Add(spriteid, left, top, right, bottom, texStair);
		}
		sprite_reader.close();

		ifstream animation_reader;
		animation_reader.open(L"Animation\\Stair.txt");
		if (!animation_reader)
		{
			DebugOut(L"[ERROR]");
		}
		int animation_id;
		string line;
		int sprite_id;
		int frameTime;
		while (getline(animation_reader, line))
		{
			core::LPANIMATION ani = new core::CAnimation();
			istringstream iss(line, istringstream::in);
			iss >> animation_id;
			while (iss >> sprite_id >> frameTime)
				ani->Add(sprite_id, frameTime);
			animations->Add(animation_id, ani);
		}
		animation_reader.close();
	}
	void Stair::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		B_left = x;
		B_top = y;
		B_right = B_left + 32;
		B_bottom = B_top + 32;
	}
	void Stair::Render()
	{
		animations[status]->Render(1, -1, x, y);
	}


}
