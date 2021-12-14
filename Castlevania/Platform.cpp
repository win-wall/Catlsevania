#include "Platform.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "Ground.h"

namespace item
{
	Platform::Platform()
	{
		Add_Animation_into_state(PLATFORM_ANIMATION);
		this->vx = -0.1f;
		this->Orientation_x = -1;
	}
	void Platform::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURE_PLATFORM, L"Textures\\Items\\Platform.png", transparentColor);
		LPDIRECT3DTEXTURE9 textPlatform = textures->Get(ID_TEXTURE_PLATFORM);

		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Platform.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom)
		{
			sprites->Add(spriteid, left, top, right, bottom, textPlatform);
		}
		sprite_reader.close();
		ifstream animation_reader;
		animation_reader.open(L"Animation\\Platform.txt");
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
	void Platform::Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game, vector<core::LPGAMEOBJECT>* coObject)
	{
		if (Orientation_x > 0)
			vx = 0.05;
		else
			vx = -0.05;
		core::CGameObject::Update(dt);
		if (x > end_x && Orientation_x == 1)
		{
			Orientation_x = Orientation_x * -1;
		}
		if (x < start_x && Orientation_x == -1)
		{
			Orientation_x = Orientation_x * -1;
		}
		vector<core::LPCOLLISIONEVENT> coEvents;
		vector<core::LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObject, coEvents);
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
			//Khi va chạm thì sẽ đẩy ra 1 chút
			x += min_tx * dx + nx * 0.1f;
			y += min_ty * dy + ny * 0.1f;
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				core::LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<static_object::Ground*>(e->obj))
				{
					if(nx!=0)
					{
						Orientation_x = Orientation_x * -1;
					}
				}
				else
				{
					x += dx;
					y += dy;
				}
			}
		}
	}
	void Platform::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		B_top = y;
		B_left = x;
		B_right = x + 64;
		B_bottom = y + 16;
	}
	void Platform::Render()
	{
		animations[status]->Render(1, -1, x, y);
	}


}
