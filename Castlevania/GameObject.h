#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <algorithm>

#include "Utils.h"
#include "Animation.h"
#include "Texture.h"
#include "Sprite.h"
namespace core
{
	struct CollisionEvent;
	typedef CollisionEvent* LPCOLLISIONEVENT;

	class CGameObject;
	typedef CGameObject* LPGAMEOBJECT;

	struct CollisionEvent
	{
		LPGAMEOBJECT obj;
		float t, nx, ny;

		CollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL)
		{
			this->t = t;
			this->nx = nx;
			this->ny = ny;
			this->obj = obj;
		}

		static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
		{
			return a->t < b->t;
		}
	};
	class CGameObject
	{
	public:
			float x, y, dx, dy,vx,vy;
			int Orientation_x;

			int status;
			
			DWORD dt;

			bool Is_Enable;
			int Id_item_that_obj_carry;

			vector<LPANIMATION> animations;
	////////////////////
	////////SET/////////
	////////////////////
		void SetPosition(float new_position_x, float new_position_y) { this->x = new_position_x; this->y = new_position_y; }
		void SetSpeed(float new_speed_vx, float new_speed_vy) { this->vx = new_speed_vx; this->vy = new_speed_vy; }
		void SetOrientation_x(int new_Orientation_x) { this->Orientation_x = new_Orientation_x; }
		void SetStatus(int new_Status) { this->status = new_Status; }
		void SetEnable(bool is_enable) { this->Is_Enable = is_enable; }
		void Set_Id_item_that_obj_carry(int id_item_that_obj_carry) { this->Id_item_that_obj_carry = id_item_that_obj_carry; }
	/////////////////////
	////////CONTRUSTOR///
	/////////////////////
		CGameObject();
	/////////////////////
	/////////GET/////////
	/////////////////////
		void GetPosition(float& get_position_x, float& get_position_y) { get_position_x = this->x; get_position_y = this->y; }
		void GetOrientation_x(int& get_Orientation_x) { get_Orientation_x = this->Orientation_x; }
		void GetStatus(int& get_status) { get_status = this->status; }
		void GetEnable(bool& get_enable) { get_enable = this->Is_Enable; }
		void GetSpeed(float& get_speed_vx, float& get_speed_vy) { get_speed_vx = this->vx; get_speed_vy = this->vy; }
	/////////////////////
	////////METHODS//////
	/////////////////////
		void RenderBBox();
		//xét va chạm giữa whip và candle, giữa whip và break wall, giữa simon với item, simon và stair
		bool AABB(
			float left_obj, float top_obj, float right_obj, float bottom_obj,
			float left_other, float top_other, float right_other, float bottom_other
		);
		void SweptAABB(
			float ml, float mt, float mr, float mb,
			float dx, float dy,
			float sl, float st,
			float sr, float sb,
			float& t, float& nx, float& ny);
		LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
		void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
		void FilterCollision(
			vector<LPCOLLISIONEVENT>& coEvents,
			vector<LPCOLLISIONEVENT>& coEventsResult,
			float& min_tx,
			float& min_ty,
			float& nx,
			float& ny);

		void Add_Animation_into_state(int aniID)
		{
			LPANIMATION ani = CAnimations::GetInstance()->Get(aniID);
			animations.push_back(ani);
		}
		virtual void GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)=0;
		virtual void Render() = 0;
		virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* List_Objects_In_Game = NULL, vector<LPGAMEOBJECT>* coObject = NULL);
		
		virtual void LoadResources(CTexture*& textures, CSprites*& sprites, CAnimations*& animations) = 0;
	};
	
}

