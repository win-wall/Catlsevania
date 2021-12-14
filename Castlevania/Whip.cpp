#include "Whip.h"
#include <fstream>
#include <iostream>
#include <sstream>
namespace simon
{
	Whip::Whip() :CGameObject()
	{
		Add_Animation_into_state(NORMAL_WHIP_ANIMATION);
		Add_Animation_into_state(SHORT_WHIP_ANIMATION);
		Add_Animation_into_state(MORNING_STAR_WHIP_ANIMATION);

		SetStatus(NORMAL_WHIP);
	}
	void Whip::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURE_WHIP, L"Textures\\Whip.png", transparentColor);
		LPDIRECT3DTEXTURE9 texWhip = textures->Get(ID_TEXTURE_WHIP);
		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Whip.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom)
		{
			sprites->Add(spriteid, left, top, right, bottom, texWhip);
		}
		sprite_reader.close();

		ifstream animation_reader;
		animation_reader.open(L"Animation\\Whip.txt");
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

	void Whip::Render(int currentID)
	{
		animations[status]->RenderByID(currentID, Orientation_x, x, y);
		RenderBBox();
	}
	void Whip::SetWhipPosition(float simon_position_x, float simon_position_y, bool isStand)
	{
		//vì sprite của simon là 70x70 và whip là 240x70
		//vị trí tay đánh frame 1 là cách vị trí top-left là x=60
		//vị trí frame 1 của whip cách vị trí top-left là x=150
		//=>150-60=90f
		//tương tự cho y
		simon_position_x -= 90.0f;
		if (isStand == false)
			simon_position_y += 15.0f;

		SetPosition(simon_position_x, simon_position_y);
	}
	bool Whip::CheckCollisionWhipAndStuff(float obj_left, float obj_top, float obj_right, float obj_bottom)
	{
		float whip_left,
			whip_top,
			whip_right,
			whip_bottom;

		GetBBox(whip_left, whip_top, whip_right, whip_bottom);

		return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
	}
	void Whip::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		//BBOX cho Morning_star là 80x15 còn short whip và normal whip là 50x15
		B_top = y + 18;
		B_bottom = B_top + 16;
		if (Orientation_x < 0)
		{
			if (status != MORNING_STAR_WHIP)
				B_left = x + 52;
			else B_left = x + 22;
		}
		else if (Orientation_x > 0)
		{
			if (status != MORNING_STAR_WHIP)
				B_left = (240 - 50) - 50 + x;
			else B_left = (240 - 20) - 80 + x;
		}
		if (status != MORNING_STAR_WHIP)
			B_right = B_left + 50;
		else  B_right = B_left + 80;
	}
}