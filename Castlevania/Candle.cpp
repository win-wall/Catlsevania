#include "Candle.h"
#include <fstream>
#include <iostream>
#include <sstream>
namespace static_object
{
	Candle::Candle() :CGameObject()
	{
		Add_Animation_into_state(BIG_CANDLE_ANIMATION);
		Add_Animation_into_state(SMALL_CANDLE_ANIMATION);
	}
	void Candle::Update(DWORD dt, vector<core::LPGAMEOBJECT>* Objects, vector<core::LPGAMEOBJECT>* coObject)
	{
		if (status == CANDLE_DESTROY)
		{
			SetEnable(false);


			if (Id_item_that_obj_carry != -1)
			{
				Item* item = new Item();
				item->SetItem(Id_item_that_obj_carry);
				item->SetEnable(true);
				item->SetPosition(x, y);
				Objects->push_back(item);
			}
		}
	}

	void Candle::Render()
	{
		if(status!=CANDLE_DESTROY)
		animations[status]->Render(1, -1, x, y);
	}
	void Candle::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		if (status == BIG_CANDLE)
		{
			//kích thước sprite của big_candle là 32*64
			B_left = x;
			B_top = y;
			B_right = B_left + 32;
			B_bottom = B_top + 64;
		}
		else if (status == SMALL_CANDLE)
		{
			//kích thước sprite của big_candle là 16*32
			B_left = x;
			B_top = y;
			B_right = B_left + 16;
			B_bottom = B_top + 32;
		}
	}
	void Candle::SetStatus(int Status)
	{
		CGameObject::SetStatus(Status);
	}
	void Candle::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		//vì nền là màu xám có R,G,B là 116,116,116
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURE_CANDLE,L"Textures\\Candle.png", transparentColor);
		LPDIRECT3DTEXTURE9 texCandle = textures->Get(ID_TEXTURE_CANDLE);

		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Candle.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom)
		{
			sprites->Add(spriteid, left, top, right, bottom, texCandle);
		}
		sprite_reader.close();
		ifstream animation_reader;
		animation_reader.open(L"Animation\\Candle.txt");
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