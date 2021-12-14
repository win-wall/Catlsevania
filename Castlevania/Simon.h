#pragma once
#include <vector>

#include "GameObject.h"
#include "Utils.h"
#include "Candle.h"
#include "Ground.h"
#include "Whip.h"
#include "SubWeapon.h"
#include "ChangeScene.h"
#include "Bat.h"
#include "Black_Knight.h"
#include "Ghost.h"
namespace simon
{
	class Simon:public core::CGameObject
	{
		Whip* whip;
		
		int Score;
		int Mana;
		int Life;
		int Subweapon;
		int HP;

		bool isUnTouchAble = false;
		DWORD UnTouchAble_time_start = 0;
		
	public:
		bool isStand = true;
		bool isOntheGround = false;
		bool isFalling = false;
		bool isOnStair = false;
		bool isMoveUp = false;
		bool isMoveDown = false;
		int stairDirection = 0;//1:trái dưới - phải trên, -1: trái trên - phải dưới
		bool IsJumping = false;

		core::LPGAMEOBJECT stairCollided = nullptr; // lưu bậc thang va chạm với simon -> để xét vị trí chuẩn trong hàm positioncorection
		int changeScene = -1;
		Simon();

		virtual void GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom);
		virtual void Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game = NULL, vector<core::LPGAMEOBJECT>* coObjects = NULL);
		void Render();
		void SetStatus(int status);
		virtual void LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations);
		//////////////////
		///////GET////////
		//////////////////
		int GetScore() { return this->Score; }
		int GetMana() { return this->Mana; }
		int	GetLife() { return this->Life; }
		int GetHP() { return this->HP; }
		int GetSubweapon() { return this->Subweapon; }
		bool Get_Is_Stand() { return this->isStand; }
		bool Get_Isontheground() { return this->isOntheGround; }
		bool Get_Is_Falling() { return this->isFalling; }
		bool Get_Is_Onstair() { return this->isOnStair; }
		bool Get_Is_Jumping() { return this->IsJumping; }

		int Get_Change_Scene() { return this->changeScene; }
		int Get_Stair_Direction() { return this->stairDirection; }
		bool Get_Is_On_Stair() { return this->isOnStair; }
		bool Get_Is_Move_Up() { return this->isMoveUp; }
		bool Get_Is_Move_Down() { return this->isMoveDown; }
		core::LPGAMEOBJECT GetStairCollided() { return this->stairCollided; }
		///////////////////
		///////SET/////////
		///////////////////
		void SetScore(int score) { this->Score = score; }
		void SetMana(int mana) { this->Mana = mana; }
		void SetLife(int life) { this->Life = life;}
		void SetHP(int hp) { this->HP = hp; }
		void SetSubweapon(int id_subweapon) { this->Subweapon = id_subweapon; }
		void Set_Is_Stand(bool true_false) { this->isStand=true_false; }
		void Set_Isontheground(bool true_false) { this->isOntheGround= true_false; }
		void Set_Is_Falling(bool true_false) {this->isFalling= true_false; }
		void Set_Is_Jumping(bool true_false) { this->IsJumping= true_false; }

		void Set_Change_Scene(int x) { this->changeScene = x; }
		void Set_On_Stair(bool x) { this->isOnStair = x; }
		/////////////////////////////////////////////////////
		/////CHECK COLLISION SIMON WITH OTHER STUFF//////////
		/////////////////////////////////////////////////////
		bool CheckCollisionSimonAndItem(float top, float left, float right, float bottom);
		bool CheckCollisionSimonAndStair(vector<core::LPGAMEOBJECT> *listStair);
		
		bool CheckChangeScene(vector<static_object::LPCHANGESCENEOBJ>* listChangeScene);
		////////////////////////////////////
		/////CĂN CHỈNH VỊ TRI CHO SIMON/////
		////////////////////////////////////
		void PositionCorrection(int prevState = -1);
		/////////////////////////////////////
		/////////GIU SIMON DUNG YEN TRN BAC THANg////
		/////////////////////////////////////////////
		void StandOnStair();
		/////////////////////////////////////////
		////////////Bật điếm tg untouchable//////
		/////////////////////////////////////////
		void StartUnTouchAble() { isUnTouchAble = true; UnTouchAble_time_start = GetTickCount(); }
		bool Get_isTouchAble() { return isUnTouchAble; }
	};
}

