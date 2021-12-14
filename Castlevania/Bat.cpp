#include "Bat.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Item.h"
namespace enemy
{
	Bat::Bat()
	{
		Add_Animation_into_state(BAT_IDLE_ANIMATION);
		Add_Animation_into_state(BAT_ATTACK_ANIMATION);
		SetStatus(BAT_IDLE);
	}
	void Bat::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		//vì nền là màu xám có R,G,B là 116,116,116
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURE_BAT, L"Textures\\Enemy\\Bat.png", transparentColor);
		LPDIRECT3DTEXTURE9 texBat = textures->Get(ID_TEXTURE_BAT);

		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Bat.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom)
		{
			sprites->Add(spriteid, left, top, right, bottom, texBat);
		}
		sprite_reader.close();
		ifstream animation_reader;
		animation_reader.open(L"Animation\\Bat.txt");
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
	void Bat::Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game, vector<core::LPGAMEOBJECT>* coObject)
	{
		if(status==BAT_DESTROY)
		{
			this->SetEnable(false);
			if (Id_item_that_obj_carry != -1)
			{
				static_object::Item* item = new static_object::Item();
				item->SetItem(Id_item_that_obj_carry);
				item->SetEnable(true);
				item->SetPosition(x, y);
				List_Objects_In_Game->push_back(item);
			}
			return;
		}
		core::CGameObject::Update(dt);
		
		x += dx;
		y += dy;
		
	}
	void Bat::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		if(status==BAT_ATTACK)
		{
			B_left = x;
			B_top = y;
			B_right = x + 32;
			B_bottom = y + 32;
		}
	}
	void Bat::Is_Simon_in_the_target(float sx, float sy)
	{
		if(x+182>sx&&y+50>=sy &&Simon_in_the_zone==false)
		{
			Simon_in_the_zone = true;
			SetStatus(BAT_ATTACK);
			SetOrientation_x(1);
			return;
		}
		else if(x-182<dx&&y+50>=sy)
		{
			Simon_in_the_zone = true;
			SetStatus(BAT_ATTACK);
			SetOrientation_x(-1);
			return;
		}
	}
	void Bat::Render()
	{
		if (status != BAT_DESTROY)
		{
			animations[status]->Render(1, Orientation_x, x, y);
		}
	}
	void Bat::SetStatus(int status)
	{
		core::CGameObject::SetStatus(status);
		switch (status)
		{
		case BAT_IDLE:
			vx = 0;
			vy = 0;
			Orientation_x = -1;
			break;
		case BAT_ATTACK:
			if (Orientation_x > 1)
			{
				vx = -0.2f;
			}
			else
				vx = 0.2f;
			if (y - start_y >= 64.0f)
				vy = 0;
			else
			vy = 0.2f;
			break;
		case BAT_DESTROY:
			vx = 0;
			break;
		}
	}


}
