#include "Ghost.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Item.h"
namespace enemy
{
	Ghost::Ghost()
	{
		Add_Animation_into_state(GHOST_ANIMATION);
		SetStatus(GHOST_ATTACK);
	}
	void Ghost::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURE_GHOST, L"Textures\\Enemy\\Ghost.png", transparentColor);
		LPDIRECT3DTEXTURE9 texGhost = textures->Get(ID_TEXTURE_GHOST);
		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Ghost.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom)
		{
			sprites->Add(spriteid, left, top, right, bottom, texGhost);
		}
		sprite_reader.close();
		ifstream animation_reader;
		animation_reader.open(L"Animation\\Ghost.txt");
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
	void Ghost::Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game, vector<core::LPGAMEOBJECT>* coObject)
	{
		if(status==GHOST_DESTROY)
		{
			this->SetEnable(false);
			return;
		}
		core::CGameObject::Update(dt);
		x += dx;
		y += dy;
		if(state1==true && Orientation_x<0)
		{
			vx = -0.09f;
		}
		else if(state1==true&&Orientation_x>0)
		{
			vx = 0.09f;
		}
		if (sy - this->y > 0)
		{
			vy = 0.02f;
		}
		else if (sy - this->y == 0)
		{
			vy = 0;
		}
		else if (sy - this->y < 0&&state2==false)
		{
			vy = -0.02f;
		}
		if(state1==true)
		{
			if(x-sx<-30 && Orientation_x==-1 && state2==false)
			{
				Orientation_x = Orientation_x * -1;
				state2 = true;
			}
			else if(x-sx>30&&Orientation_x==1&&state2==true)
			{
				Orientation_x = Orientation_x * -1;
				state2 = false;
				vy = -0.05f;
				
			}
		}
	}
	void Ghost::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		if (status == GHOST_ATTACK)
		{
			B_left = x;
			B_top = y;
			B_right = x + 32;
			B_bottom = y + 32;
		}
	}
	void Ghost::Is_Simon_in_the_target(float sx, float sy, bool x)
	{
		this->sx=sx;
		this->sy=sy;
		if(-sx+this->x>192)
		{
			Simon_in_the_zone = true;
			state1 = true;
			SetStatus(GHOST_ATTACK);
			SetOrientation_x(-1);
			return;
		}
		
	}
	void Ghost::Render()
	{
		if(status!=GHOST_INACTIVE && status!=GHOST_DESTROY)
		{
			animations[status]->Render(1, Orientation_x, x, y);
		}
	}
	void Ghost::SetStatus(int status)
	{
		core::CGameObject::SetStatus(status);
		switch (status)
		{
		case GHOST_INACTIVE:
			vx = 0;
			vy = 0;
			Orientation_x = -1;
			break;
		case GHOST_ATTACK:
			vx = -0.05f;
			Orientation_x = -1;
			break;
		}
		
		}
}



