#include "SubWeapon.h"
#include <fstream>
#include <iostream>
#include <sstream>
namespace simon
{
	SubWeapon::SubWeapon()
	{
		Add_Animation_into_state(WEAPONS_STOP_WATCH_ANIMATION);
		Add_Animation_into_state(WEAPONS_DAGGER_ANIMATION);
		Add_Animation_into_state(WEAPONS_AXE_ANIMATION);
		Add_Animation_into_state(WEAPONS_HOLY_WATER_ANIMATION);
		Add_Animation_into_state(WEAPONS_BOOMERANG_ANIMATION);
		Add_Animation_into_state(USING_HOLY_WATER_ANIMATION);
		status = -1;
	}
	SubWeapon::~SubWeapon()
	{
		
	}
	void SubWeapon::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURES_WEAPONS_AXE, L"Textures\\SubWeapons\\Axe.png", transparentColor);
		textures->Add(ID_TEXTURES_WEAPONS_HOLY_WATER, L"Textures\\SubWeapons\\HolyWater.png", transparentColor);
		textures->Add(ID_TEXTURES_WEAPONS_BOOMERANG, L"Textures\\SubWeapons\\Boomerang.png", transparentColor);
		textures->Add(ID_TEXTURES_WEAPONS_DAGGER, L"Textures\\SubWeapons\\Dagger.png", transparentColor);
		LPDIRECT3DTEXTURE9 texAxe = textures->Get(ID_TEXTURES_WEAPONS_AXE);
		LPDIRECT3DTEXTURE9 texHolyWater = textures->Get(ID_TEXTURES_WEAPONS_HOLY_WATER);
		LPDIRECT3DTEXTURE9 texBoomerang = textures->Get(ID_TEXTURES_WEAPONS_BOOMERANG);
		LPDIRECT3DTEXTURE9 texDagger = textures->Get(ID_TEXTURES_WEAPONS_DAGGER);
		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\SubWeapons.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom, id_tex;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom >> id_tex)
		{
			switch (id_tex)
			{
			case ID_TEXTURES_WEAPONS_AXE:
				sprites->Add(spriteid, left, top, right, bottom, texAxe);
				break;
			case ID_TEXTURES_WEAPONS_HOLY_WATER:
				sprites->Add(spriteid, left, top, right, bottom, texHolyWater);
				break;
			case ID_TEXTURES_WEAPONS_BOOMERANG:
				sprites->Add(spriteid, left, top, right, bottom, texBoomerang);
				break;
			case ID_TEXTURES_WEAPONS_DAGGER:
				sprites->Add(spriteid, left, top, right, bottom, texDagger);
				break;
			}
		}
		sprite_reader.close();

		ifstream animation_reader;
		animation_reader.open(L"Animation\\SubWeapons.txt");
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
	void SubWeapon::Update(DWORD dt, vector<core::LPGAMEOBJECT>* List, vector<core::LPGAMEOBJECT>* coObject)
	{
		if(Is_Holy_Water_On_The_Ground==true && GetTickCount()-Holy_Water_On_The_Ground_Counter>1000)
		{
			Is_Holy_Water_On_The_Ground = false;
			Holy_Water_On_The_Ground_Counter = 0;
			SetEnable(false);
			return;
		}
		core::CGameObject::Update(dt);
		switch(status)
		{
		case STOP_WATCH:
			SetEnable(false);
			break;
		case FLYING_AXE:
			vy += 0.02f;
			break;
		case BOOMERANG:
			vx += (-Orientation_x * 0.01f);
			break;
		case HOLY_WATER:
			vy += 0.01f;
			break;
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
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				core::LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<static_object::Candle*>(e->obj))
				{
					static_object::Candle* candle = dynamic_cast<static_object::Candle*>(e->obj);
					candle->SetStatus(CANDLE_DESTROY);
					SetEnable(false);
				}
				else if (dynamic_cast<static_object::Ground*>(e->obj))
				{
					if (status == HOLY_WATER && e->ny == -1)
						SetState(USING_HOLY_WATER);
					else
					{
						x += dx;
						y += dy;
					}
				}
			}
		}
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	void SubWeapon::Render()
	{
		if (status != STOP_WATCH)
			animations[status]->Render(1, Orientation_x, x, y);
	}
	void SubWeapon::SetState(int state)
	{
		CGameObject::SetStatus(state);

		switch (state)
		{
		case STOP_WATCH:
			break;
		case DAGGER:
			vx = Orientation_x * WEAPONS_DAGGER_SPEED;
			vy = 0;
			break;
		case FLYING_AXE:
			vx = Orientation_x * WEAPONS_AXE_SPEED_X;
			vy = -WEAPONS_AXE_SPEED_Y;
			break;
		case HOLY_WATER:
			vx = Orientation_x * WEAPONS_HOLY_WATER_SPEED_X;
			vy = -WEAPONS_HOLY_WATER_SPEED_Y;
			break;
		case BOOMERANG:
			vx = Orientation_x * WEAPONS_BOOMERANG_SPEED;
			vy = 0;
			break;
		case USING_HOLY_WATER:
			vx = 0;
			vy = 0;
			StartHolyWaterEffect();
			break;
		default:
			break;
		}
	}
	void SubWeapon::GetBBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;

		switch (status)
		{
		case STOP_WATCH:
			right = left + 26;
			bottom = top + 28;
		case DAGGER:
			right = left + 32;
			bottom = top + 18;
		case FLYING_AXE:
			right = left + 30;
			bottom = top + 28;
		case HOLY_WATER:
			right = left + 32;
			bottom = top + 32;
		case BOOMERANG:
			right = left + 30;
			bottom = top + 28;
		case USING_HOLY_WATER:
			right = left + 30;
			bottom = top + 28;
		default:
			break;
		}
	}
}