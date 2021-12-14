#include "Black_Knight.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "Item.h"

namespace enemy
{
	Black_Knight::Black_Knight()
	{
		Add_Animation_into_state(BLACK_KNIGHT_ANIMATION);
		this->vx = -0.1f;
		this->Orientation_x = -1;
	}
	void Black_Knight::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		//vì nền là màu xám có R,G,B là 116,116,116
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURE_BLACK_KNIGHT, L"Textures\\Enemy\\Black_Knight.png", transparentColor);
		LPDIRECT3DTEXTURE9 texBlackKnight = textures->Get(ID_TEXTURE_BLACK_KNIGHT);

		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Black_Knight.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom)
		{
			sprites->Add(spriteid, left, top, right, bottom, texBlackKnight);
		}
		sprite_reader.close();
		ifstream animation_reader;
		animation_reader.open(L"Animation\\Black_Knight.txt");
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
	void Black_Knight::Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game, vector<core::LPGAMEOBJECT>* coObject)
	{
		if(status == BLACK_KNIGHT_DESTROY)
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
		if (Orientation_x > 0)
			vx = 0.05;
		else
			vx = -0.05;
		
		core::CGameObject::Update(dt);
		if (Simon_in_the_zone == false)
		{
			if (x > end_x  && Orientation_x == 1)
			{
				Orientation_x = Orientation_x * -1;
			}
			if(x<start_x&&Orientation_x==-1)
			{
				Orientation_x = Orientation_x * -1;
			}
		}
		else
		{
			if(x<destination_x&&Orientation_x==-1)
			{
				Orientation_x = Orientation_x * -1;
			}
			if(x>destination_x&&Orientation_x==1)
			{
				Orientation_x = Orientation_x * -1;
			}
		}
		if(IsOntheGround==false)
		{
			vy += 0.001f*dt;
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
					if (ny < 0)
					{
						vy = 0;
						IsOntheGround = true;
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
	void Black_Knight::Is_Simon_in_the_target(float sx, float sy,bool x)
	{
		if (x == true)
		{
			Simon_in_the_zone = false;
			Set_destination_x(0);
			return;
		}
		if (start_x <= sx && end_x >= sx &&y<sy+10&&y+64>=sy+10)
		{
			
				Simon_in_the_zone = true;
				Set_destination_x(sx);
		}
		else
		{
			Simon_in_the_zone = false;
			Set_destination_x(0);
		}
	}
	void Black_Knight::Render()
	{
		animations[status]->Render(1, Orientation_x, x, y);
	}
	void Black_Knight::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		B_top = y;
		B_left = x;
		B_right = x + 32;
		B_bottom = y + 64;
	}
	

}
