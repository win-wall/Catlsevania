#include "HUD.h"
namespace HUD
{
	HUD::HUD(core::SceneManager* scenes, core::CGame* game)
	{
		this->scenes = scenes;
		this->game = game;
		this->simons = scenes->Get_Simon();

		Time = 0;
	}
	 HUD::~HUD()
	 {
		 
	 }

	void HUD::Init()
	{
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		core::CTexture* texture = core::CTexture::GetInstance();
		texture->Add(ID_TEXTURES_HP, L"Textures\\HP.png", transparentColor);
		texture->Add(ID_TEXTURES_RECT, L"Textures\\Rect.png", transparentColor);

		LPDIRECT3DTEXTURE9 texHP = texture->Get(ID_TEXTURES_HP);
		LPDIRECT3DTEXTURE9 texRECT = texture->Get(ID_TEXTURES_RECT);

		for(int i=0;i<16;i++)
		{
			core::CSprite* player = new core::CSprite(100,0,0,8,15,texHP);
			playerHp.push_back(player);

			core::CSprite* lose = new core::CSprite(101, 8, 0, 16, 15, texHP);
			loseHP.push_back(lose);

			core::CSprite* enemy = new core::CSprite(102, 16, 0, 24, 15, texHP);
			enemyHP.push_back(enemy);
		}

		subWeaponBox = new core::CSprite(110, 0, 0, 95, 40, texRECT);

		core::CSprites* sprite = core::CSprites::GetInstance();
		subWeaponList.push_back(sprite->Get(40001));
		subWeaponList.push_back(sprite->Get(40002));
		subWeaponList.push_back(sprite->Get(40003));
		subWeaponList.push_back(sprite->Get(40004));
		subWeaponList.push_back(sprite->Get(40005));

		font = NULL;
		AddFontResourceEx(L"Font\\prstart.ttf", FR_PRIVATE, NULL);

		HRESULT hr = D3DXCreateFont(game->GetDirect3DDevice(), 16, 0, FW_NORMAL
			, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS
			, ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &font);
		if(hr!=DI_OK)
		{
			DebugOut(L"[ERROR]	CREATE FONT FAILED\n");
			return;
		}
		information = "SCORE-000000 TIME 0000 SCENE 00\n";
		information += "PLAYER					-00\n";
		information += "ENEMY					-00\n";
	}
	void HUD::Update(DWORD dt)
	{
		Score_Simon = simons->GetScore();
		Mana_Simon = simons->GetMana();
		Life_Simon = simons->GetLife();
		Subweapon_Simon = simons->GetSubweapon();
		HP_Simon = simons->GetHP();
		Scene = scenes->Get_ID_SCENE();
		Time += dt;

		int RemainTime = 1000 - Time / 1000;

		string score_str = to_string(Score_Simon);
		while (score_str.length() < 6) score_str = "0" + score_str;

		string time_str = to_string(RemainTime);
		while (time_str.length() < 4) time_str = "0" + time_str;

		string scene_str = to_string(Scene);
		while (scene_str.length() < 2) scene_str = "0" + scene_str;

		string mana_str = to_string(Mana_Simon);
		while (mana_str.length() < 2) mana_str = "0" + mana_str;
		
		string life_str = to_string(Life_Simon);
		while (life_str.length() < 2) life_str = "0" + life_str;
		

		//update information
		information = "SCORE-" + score_str + " TIME " + time_str + " SCENE" + scene_str + "\n";
		information += "PLAYER                  -" + mana_str + "\n";
		information += "ENEMY                   -" + life_str + "\n";
	}
	void HUD::Render()
	{
		RECT rect;
		SetRect(&rect, 0, 5, SCREEN_WIDTH, 64);
		if(font!=NULL)
		{
			font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
			
		}
		//draw subweaponbox
		subWeaponBox->Draw(0, -1, 288, 20);
		if(Subweapon_Simon!=-1)
		{
			subWeaponList[Subweapon_Simon]->Draw(0, -1, 303, 25);
			
		}
		
		for(int i=0;i<HP_Simon;i++)
		{
			playerHp[i]->Draw(0, -1, 105 + i * 9, 20);
			enemyHP[i]->Draw(0, -1, 106 + i * 9, 36);
			
		}
		for(int i =HP_Simon;i<16;i++)
		{
			loseHP[i]->Draw(0, -1, 105 + i * 9, 20);
		}
		for(int i =0;i<16;i++)
		{
			enemyHP[i]->Draw(0,-1,105+i*9,36);
		}
	}


}