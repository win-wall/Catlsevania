#include "Item.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
namespace static_object
{
	Item::Item():CGameObject()
	{
		vy = 0.2f; //tốc độ rơi của item
		Add_Animation_into_state(STOP_WATCH_ANIMATION);
		Add_Animation_into_state(Dagger_ANIMATION);
		Add_Animation_into_state(FLYING_AXE_ANIMATION);
		//RIP AND TEAR
		//Check :3 in untils.h for a little surprise
		Add_Animation_into_state(DOOM_SLAYER_FAVORITE_WATER_TO_RIP_AND_TEAR_ALL_THE_DEMON);
		Add_Animation_into_state(BOOMMERANG_ANIMATION);
		Add_Animation_into_state(SMALL_HEART_ANIMATION);
		Add_Animation_into_state(LARGE_HEART_ANIMATION);
		Add_Animation_into_state(CHAIN_ANIMATION);
		time_disappear = -1;
	}
	void Item::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURES_ITEM_STOP_WATCH, L"Textures\\Items\\StopWatch.png", transparentColor);
		LPDIRECT3DTEXTURE9 texStopWatch = textures->Get(ID_TEXTURES_ITEM_STOP_WATCH);
		textures->Add(ID_TEXTURES_ITEM_DAGGER, L"Textures\\Items\\Dagger.png", transparentColor);
		LPDIRECT3DTEXTURE9 texDagger = textures->Get(ID_TEXTURES_ITEM_DAGGER);
		textures->Add(ID_TEXTURES_ITEM_FLYING_AXE, L"Textures\\Items\\Axe.png", transparentColor);
		LPDIRECT3DTEXTURE9 texAxe = textures->Get(ID_TEXTURES_ITEM_FLYING_AXE);
		textures->Add(ID_TEXTURES_ITEM_HOLY_WATER, L"Textures\\Items\\HolyWater.png", transparentColor);
		LPDIRECT3DTEXTURE9 texHolyWater = textures->Get(ID_TEXTURES_ITEM_HOLY_WATER);
		textures->Add(ID_TEXTURES_ITEM_BOOMERANG, L"Textures\\Items\\Boomerang.png", transparentColor);
		LPDIRECT3DTEXTURE9 texBoomerang = textures->Get(ID_TEXTURES_ITEM_BOOMERANG);
		textures->Add(ID_TEXTURES_ITEM_SMALL_HEART, L"Textures\\Items\\SmallHeart.png", transparentColor);
		LPDIRECT3DTEXTURE9 texSmallHeart = textures->Get(ID_TEXTURES_ITEM_SMALL_HEART);
		textures->Add(ID_TEXTURES_ITEM_LARGE_HEART, L"Textures\\Items\\LargeHeart.png", transparentColor);
		LPDIRECT3DTEXTURE9 texLargeHeart = textures->Get(ID_TEXTURES_ITEM_LARGE_HEART);
		textures->Add(ID_TEXTURES_ITEM_CHAIN, L"Textures\\Items\\Chain.png", transparentColor);
		LPDIRECT3DTEXTURE9 texChain = textures->Get(ID_TEXTURES_ITEM_CHAIN);
		
		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Item.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom,id_tex;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom >>id_tex)
		{
			switch (id_tex)
			{
			case ID_TEXTURES_ITEM_STOP_WATCH:
				sprites->Add(spriteid, left, top, right, bottom, texStopWatch);
				break;
			case ID_TEXTURES_ITEM_DAGGER:
				sprites->Add(spriteid, left, top, right, bottom, texDagger);
					break;
			case ID_TEXTURES_ITEM_FLYING_AXE:
				sprites->Add(spriteid, left, top, right, bottom, texAxe);
				break;
			case ID_TEXTURES_ITEM_HOLY_WATER:
				sprites->Add(spriteid, left, top, right, bottom, texHolyWater);
				break;
			case ID_TEXTURES_ITEM_BOOMERANG:
				sprites->Add(spriteid, left, top, right, bottom, texBoomerang);
				break;
			case ID_TEXTURES_ITEM_SMALL_HEART:
				sprites->Add(spriteid, left, top, right, bottom, texSmallHeart);
				break;
			case ID_TEXTURES_ITEM_LARGE_HEART:
				sprites->Add(spriteid, left, top, right, bottom, texLargeHeart);
				break;
			case ID_TEXTURES_ITEM_CHAIN:
				sprites->Add(spriteid, left, top, right, bottom, texChain);
				break;
			}
		}
		sprite_reader.close();

		ifstream animation_reader;
		animation_reader.open(L"Animation\\Item.txt");
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
	void Item::Render()
	{
		animations[status]->Render(1, -1, x, y);
	}
	void Item::Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game, vector<core::LPGAMEOBJECT>* coObject)
	{
		if (time_disappear == -1)
		{
			time_disappear = GetTickCount();
		}
		else
		{
			DWORD now = GetTickCount();
			if(now-time_disappear>5000)
			{
				SetEnable(false);
				return;
			}
		}
		CGameObject::Update(dt);

		// Check collision between item and ground (falling on ground)
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

			y += min_ty * dy + ny * 0.1f;
			if (ny != 0) vy = 0;
		}

		// clean up collision events
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	void Item::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		B_left = x;
		B_top = y;
		switch (status)
		{
		case STOP_WATCH:
			B_right = B_left + 26;
			B_bottom = B_top + 28;
			break;
		case DAGGER:
			B_right = B_left + 32;
			B_bottom = B_top + 18;
			break;
		case FLYING_AXE:
			B_right = B_left + 30;
			B_bottom = B_top + 28;
			break;
		case HOLY_WATER:
			B_right = B_left + 28;
			B_bottom = B_top + 28;
			break;
		case BOOMERANG:
			B_right = B_left + 30;
			B_bottom = B_top + 28;
			break;
		case SMALL_HEART:
			B_right = B_left + 16;
			B_bottom = B_top + 16;
			break;
		case LAGRE_HEART:
			B_right = B_left + 24;
			B_bottom = B_top + 20;
			break;
		case CHAIN:
			B_right = B_left + 32;
			B_bottom = B_top + 32;
			break;
		}
	}
}