#include "Ground.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
namespace static_object
{
	Ground::Ground() :CGameObject()
	{
		Add_Animation_into_state(GROUND_ANIMATION_1);
		Add_Animation_into_state(GROUND_ANIMATION_2);
		Add_Animation_into_state(GROUND_ANIMATION_2_2);
		Add_Animation_into_state(GROUND_ANIMATION_3);
		Add_Animation_into_state(GROUND_ANIMATION_3_2);
		Add_Animation_into_state(GROUND_ANIMATION_4);
	}
	void Ground::Render()
	{
		animations[status]->Render(1, -1, x, y);
	}
	void Ground::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		B_left = x;
		B_top = y;
		B_right = x + 32;
		B_bottom = y + 32;
	}
	void Ground::SetStatus(int Status)
	{
		CGameObject::SetStatus(Status);
	}
	void Ground::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(0, 0, 0);
		textures->Add(ID_TEXTURE_GROUND, L"Textures\\Ground\\Ground.png",transparentColor);
		LPDIRECT3DTEXTURE9 texGround = textures->Get(ID_TEXTURE_GROUND);

		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Ground.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom)
		{
				sprites->Add(spriteid, left, top, right, bottom, texGround);
		}
		sprite_reader.close();

		ifstream animation_reader;
		animation_reader.open(L"Animation\\Ground_1.txt");
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
}