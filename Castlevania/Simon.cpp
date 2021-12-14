#include "Simon.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "Platform.h"
using namespace std;
namespace simon
{
	Simon::Simon() : CGameObject()
	{
		SetStatus(STAND);

		Add_Animation_into_state(STAND_ANIMATION);
		Add_Animation_into_state(WALK_ANIMATION);
		Add_Animation_into_state(SIT_ANIMATION);
		Add_Animation_into_state(JUMP_ANIMATION);
		Add_Animation_into_state(HIT_SIT_ANIMATION);
		Add_Animation_into_state(HIT_STAND_ANIMATION);
		Add_Animation_into_state(HIT_STAND_ANIMATION);
		Add_Animation_into_state(UPDATE_ANIMATION);
		Add_Animation_into_state(STAIR_UP_ANIMATION);
		Add_Animation_into_state(STAIR_DOWN_ANIMATION);
		Add_Animation_into_state(HIT_STAIR_UP_ANIMATION);
		Add_Animation_into_state(HIT_STAIR_DOWN_ANIMATION);
		Add_Animation_into_state(DEFLECT_ANIMATION);
		whip = new Whip();
		Mana = 14;
		Life = 3;
		Subweapon = -1;
		HP = 10;
	}
	void Simon::Update(DWORD dt, vector<core::LPGAMEOBJECT>* List_Objects_In_Game, vector<core::LPGAMEOBJECT>* coObjects)
	{
		
	
		
		core::CGameObject::Update(dt);
		//Cho tốc độ simon nhảy lên nhanh gấp 2 lần khi simon rơi xuống
		if (status != STAIR_UP && status != STAIR_DOWN)
		{
				if (vy >= 0 && vy <= SIMON_SPEED_JUMP / 2)
				{
					vy += HALF_OF_GRAVITY * dt;
				}
				else
				{
					vy += GRAVITY * dt;
					isFalling = true;
					//tránh lun giữ trạng thái ATTACK_STAND khi simon rơi xuống làm quay lại FRAME đầu.
					if (vy > 0.6f)
					{
						status = STAND;
					}
				}
			//simon rơi tự do
			if(status!=ATTACK_STAND&&status!=JUMP &&status!=DEFLECT&&vy>0.016f)
			{
				vx = 0;
				isFalling = true;
				isOntheGround = false;
			}
		}
		if(GetTickCount()-UnTouchAble_time_start>2000)
		{
			UnTouchAble_time_start = 0;
			isUnTouchAble = false;
		}
		//Xét va chạm với item
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			core::LPGAMEOBJECT obj = coObjects->at(i);
			if (dynamic_cast<static_object::Item*>(obj))
			{
				static_object::Item* e = dynamic_cast<static_object::Item*>(obj);
				float left, top, right, bottom;
				e->GetBBox(left, top, right, bottom);
				if (CheckCollisionSimonAndItem(left, top, right, bottom) == true)
				{
					obj->SetEnable(false);
					int idItem;
					obj->GetStatus(idItem);
					switch (idItem)
					{
					case STOP_WATCH:
					case DAGGER:
					case FLYING_AXE:
					case HOLY_WATER:
					case BOOMERANG:
						Subweapon = idItem;
						break;
					case SMALL_HEART:
						Mana += 1;
						break;
					case LAGRE_HEART:
						Mana += 5;
						break;
					case CHAIN:
						SetStatus(UPDATE);
						vx = 0;
						// lên đời whip
						int status;
						whip->GetStatus(status);
						if (status == NORMAL_WHIP) whip->SetStatus(SHORT_WHIP);
						else if (status == SHORT_WHIP) whip->SetStatus(MORNING_STAR_WHIP);
						break;
					default:
						break;
					}
				}
			}
			if (dynamic_cast<enemy::Black_Knight*>(obj))
			{
				enemy::Black_Knight* e = dynamic_cast<enemy::Black_Knight*>(obj);
				float left, top, right, bottom;
				e->GetBBox(left, top, right, bottom);
				if (CheckCollisionSimonAndItem(left, top, right, bottom) == true && isUnTouchAble==false)
				{
					SetStatus(DEFLECT);
					StartUnTouchAble();
					SetHP(HP - 2);
				}
			}
			if (dynamic_cast<enemy::Bat*>(obj))
			{
				enemy::Bat* e = dynamic_cast<enemy::Bat*>(obj);
				float left, top, right, bottom;
				e->GetBBox(left, top, right, bottom);
				if (CheckCollisionSimonAndItem(left, top, right, bottom) == true )
				{
					e->SetStatus(BAT_DESTROY);
				}
			}
			if (dynamic_cast<enemy::Ghost*>(obj))
			{
				enemy::Ghost* e = dynamic_cast<enemy::Ghost*>(obj);
				float left, top, right, bottom;
				e->GetBBox(left, top, right, bottom);
				if (CheckCollisionSimonAndItem(left, top, right, bottom) == true && isUnTouchAble == false)
				{
					SetStatus(DEFLECT);
					StartUnTouchAble();
					SetHP(HP - 2);
				}
			}
		}
		
		//Va chạm với cạnh màn hình
		if (x < 0)
			x = 0;
		//Xét va chạm với vật thể
		vector<core::LPCOLLISIONEVENT> coEvents;
		vector<core::LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);
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
				if (dynamic_cast<static_object::Candle*>(e->obj))
				{
					//cho phép đi qua lun candle
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
				else if (dynamic_cast<static_object::Ground*>(e->obj) )
				{
					if ((nx != 0||ny!=0) && status == STAIR_UP)
					{
						if (nx > 0)
						{
							x += nx * 0.1f;// khi lên bậc thang cuối cùng khi va chạm với ground thì giúp simon đi sâu vào ground 1 đoạn => không xét sweptAABB nữa simon đi thẳng lên luôn
						}
						else if (nx < 0)
						{
							x -= nx * 0.1f;
						}
						if (ny > 0)
						{
							y -= ny * 0.2f;// khi lên bậc thang cuối cùng khi va chạm với ground thì giúp simon đi sâu vào ground 1 đoạn => không xét sweptAABB nữa simon đi thẳng lên luôn
						}
						else if (ny < 0)
						{
							y += ny * 0.2f;
						}
					}
					if (ny < 0)
					{
						vy = 0;
						isOntheGround = true;
						isFalling = false;
					}
					
					if(status ==STAIR_DOWN)
					{
						status = STAND;
					}

					if(status==DEFLECT)
					{
						status = SIT;
					}
				}
				else if(dynamic_cast<static_object::Item*>(e->obj))
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
				else if(dynamic_cast<simon::SubWeapon*>(e->obj))
				{
					int status;
					e->obj->GetStatus(status);
					if(status==BOOMERANG)
					{
						e->obj->SetEnable(false);
					}
					else
					{
						if (e->nx != 0) x += dx;
						if (e->ny != 0) y += dy;
					}
				}
				else if (dynamic_cast<item::Platform*>(e->obj))
				{
					
					if (e->ny != 0)
					{
						isOntheGround = true;
						isFalling = false;
						vx = e->obj->vx;
						vy = 0;
					}
				}
				else if((dynamic_cast<enemy::Bat*>(e->obj) ||dynamic_cast<enemy::Black_Knight*>(e->obj)||dynamic_cast<enemy::Ghost*>(e->obj))&& isUnTouchAble==false)
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
  					SetStatus(DEFLECT);
					StartUnTouchAble();
					SetHP(HP - 2);
				}
			}
		}
		// clean up collision events
		for (int i = 0; i < coEvents.size(); i++)
			delete coEvents[i];
		if (status == ATTACK_SIT || status == ATTACK_STAND)
		{
			// lấy vị trí của simon để set vị trí cho roi
			whip->SetOrientation_x(Orientation_x);
			whip->SetWhipPosition(x, y, isStand);
			if (animations[status]->GetCurrentFrame() == animations[status]->GetFramesSize() - 1) // chỉ xét va chạm khi render tới sprite cuối cùng của roi
			{
				for (UINT i = 0; i < coObjects->size(); i++)
				{
					core::LPGAMEOBJECT obj = coObjects->at(i);
					if (dynamic_cast<static_object::Candle*>(obj))
					{
						static_object::Candle* e = dynamic_cast<static_object::Candle*> (obj);
						float left, top, right, bottom;
						e->GetBBox(left, top, right, bottom);
						if (whip->CheckCollisionWhipAndStuff(left, top, right, bottom) == true) // va chạm giữa roi và nến dùng AABB
						{
							e->SetStatus(CANDLE_DESTROY);
						}
					}
					if(dynamic_cast<enemy::Bat*>(obj))
					{
						enemy::Bat* e = dynamic_cast<enemy::Bat*>(obj);
						float left, top, right, bottom;
						e->GetBBox(left, top, right, bottom);
						if(whip->CheckCollisionWhipAndStuff(left,top,right,bottom)==true)
						{
							e->SetStatus(BAT_DESTROY);
						}
					}
					if (dynamic_cast<enemy::Black_Knight*>(obj))
					{
						enemy::Black_Knight* e = dynamic_cast<enemy::Black_Knight*>(obj);
						float left, top, right, bottom;
						e->GetBBox(left, top, right, bottom);
						if (whip->CheckCollisionWhipAndStuff(left, top, right, bottom) == true)
						{
							e->SetStatus(BLACK_KNIGHT_DESTROY);
						}
					}
					if (dynamic_cast<enemy::Bat*>(obj))
					{
						enemy::Bat* e = dynamic_cast<enemy::Bat*>(obj);
						float left, top, right, bottom;
						e->GetBBox(left, top, right, bottom);
						if (whip->CheckCollisionWhipAndStuff(left, top, right, bottom) == true)
						{
							e->SetStatus(BAT_DESTROY);
						}
					}
					if (dynamic_cast<enemy::Ghost*>(obj))
					{
						enemy::Ghost* e = dynamic_cast<enemy::Ghost*>(obj);
						float left, top, right, bottom;
						e->GetBBox(left, top, right, bottom);
						if (whip->CheckCollisionWhipAndStuff(left, top, right, bottom) == true)
						{
							e->SetStatus(GHOST_DESTROY);
						}
					}
				}
			}
		}
		
	}
	void Simon::Render()
	{
		if(isUnTouchAble)
		{
			int r = rand() % 2;
			if (r == 0)
			{
				animations[status]->Render(1, Orientation_x, x, y);
			}
			else
				animations[status]->Render(1, Orientation_x, x, y, 100);
		}
		else
		{
			animations[status]->Render(1, Orientation_x, x, y);
			if (status == ATTACK_SIT || status == ATTACK_STAND)
			{
				whip->Render(animations[status]->GetCurrentFrame());
			}
		}
	}
	void Simon::SetStatus(int status)
	{
		core::CGameObject::SetStatus(status);
		switch (status)
		{
		case STAND:
			isStand = true;
			isOntheGround = true;
			isMoveUp = false;
			isMoveDown = false;
			isOnStair = false;
			
			break;
		case WALK:
			isOnStair = false;
			if (Orientation_x > 0)
				vx = SIMON_WALK_SPEED;
			else
				vx = -SIMON_WALK_SPEED;
			break;
		case JUMP:
			isOntheGround = false;
			isOnStair = false;
			vy = -SIMON_SPEED_JUMP;
			break;
		case SIT:
			isOnStair = false;
			vx = 0;
			isStand = false;
			isOntheGround = true;
			break;
		case ATTACK_SIT:
			isOnStair = false;
			isStand = false;
			isOntheGround = true;
			animations[status]->Reset();
			animations[status]->SetAniStartTime(GetTickCount());
			break;
		case ATTACK_STAND:
		case ATTACK:
		case UPDATE:
			isOnStair = false;
			isStand = true;
			animations[status]->Reset();
			animations[status]->SetAniStartTime(GetTickCount());
			break;
		case STAIR_UP:
			isOntheGround = false;
			isStand = true;
			if (Orientation_x > 0) vx = 0.08f;
			else vx = -0.08f;
			vy = -0.08f;
			animations[status]->Reset();
			animations[status]->SetAniStartTime(GetTickCount());
			break;
		case STAIR_DOWN:
			isStand = true;
			isOntheGround = false;
			if (Orientation_x > 0) vx = 0.08f;
			else vx = -0.08f;
			vy = 0.08f;
			animations[status]->Reset();
			animations[status]->SetAniStartTime(GetTickCount());
			break;
		case DEFLECT:
			DebugOut(L"SIMON DEFLECT");
			vy = -SIMON_DEFLECT_SPEED_Y;
			if (Orientation_x > 0)
				vx = -SIMON_DEFLECT_SPEED_X;
			else
				vx = SIMON_DEFLECT_SPEED_X;
			animations[status]->Reset();
			animations[status]->SetAniStartTime(GetTickCount());
		default:
			break;
		}
	}
	void Simon::GetBBox(float& B_left, float& B_top, float& B_right, float& B_bottom)
	{
		int status;
		GetStatus(status);
		if (Orientation_x==-1 &&status !=JUMP && status != STAIR_UP)
		{
			//sprite của SIMON là 70x70, BBox là 40x64
			B_left = x + 13;
			B_top = y + 8;
			B_right = B_left + 32;
			B_bottom = B_top + 59;
		}
		else if(Orientation_x==1 && status != JUMP && status != STAIR_UP)
		{
			B_left = x + 24;
			B_top = y + 8;
			B_right = B_left + 32;
			B_bottom = B_top + 59;
		}
		
		else if(Orientation_x == -1 && status == JUMP)
		{
			B_left = x + 13;
			B_top = y + 18;
			B_right = B_left + 32;
			B_bottom = B_top + 49;
		}
		else if (Orientation_x == 1 && status == JUMP)
		{
			B_left = x + 24;
			B_top = y + 18;
			B_right = B_left + 32;
			B_bottom = B_top + 49;
		}
		else if(Orientation_x==1 &&status ==STAIR_UP)
		{
			B_left = x + 24;
			B_top = y + 8;
			B_right = B_left + 39;
			B_bottom = B_top + 59;
		}
	}
	void Simon::LoadResources(core::CTexture*& textures, core::CSprites*& sprites, core::CAnimations*& animations)
	{
		D3DCOLOR transparentColor = D3DCOLOR_XRGB(116, 116, 116);
		textures->Add(ID_TEXTURE_SIMON, L"Textures\\Simon.png", transparentColor);
		LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEXTURE_SIMON);
		ifstream sprite_reader;
		sprite_reader.open(L"Sprite\\Simon.txt");
		if (sprite_reader.fail())
		{
			DebugOut(L"[ERROR]");
		}
		int spriteid;
		int top, left, right, bottom;
		while (sprite_reader >> spriteid >> left >> top >> right >> bottom)
		{
			sprites->Add(spriteid, left, top, right, bottom, texSimon);
		}
		sprite_reader.close();

		ifstream animation_reader;
		animation_reader.open(L"Animation\\Simon.txt");
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
	bool Simon::CheckCollisionSimonAndItem(float left, float top, float right, float bottom)
	{
		float simon_left, simon_top, simon_right, simon_bottom;
		GetBBox(simon_left, simon_top, simon_right, simon_bottom);
		return AABB(simon_left, simon_top, simon_right, simon_bottom, left, top, right, bottom);
	}
	bool Simon::CheckChangeScene(vector<static_object::LPCHANGESCENEOBJ>* listChangeScene)
	{
		float simon_left, simon_top, simon_right, simon_bottom;
		float left, top, right, bottom;

		GetBBox(simon_left, simon_top, simon_right, simon_bottom);

		for (UINT i = 0; i < listChangeScene->size(); i++)
		{
			listChangeScene->at(i)->GetBBox(left, top, right, bottom);
			float t, nx, ny;
			core::CGameObject::SweptAABB(simon_left, simon_top, simon_right, simon_bottom, dx, dy, left, top, right, bottom, t, nx, ny);
			bool collision = core::CGameObject::AABB(simon_left, simon_top, simon_right, simon_bottom, left, top, right, bottom);
			if (nx != 0 || ny != 0 || collision == true)
			{
				changeScene = listChangeScene->at(i)->Get_ID_SCENE();
				return true;
			}
		}
		return false;
	}
	bool Simon::CheckCollisionSimonAndStair(vector<core::LPGAMEOBJECT>* listStair)
	{
		
		

		float simon_left, simon_top, simon_right, simon_bottom;
		GetBBox(simon_left, simon_top, simon_right, simon_bottom);
		/////////////////////////////////////////////////
		////CHINH BBOX CHI XET VA CHAM DUOI CHAN SIMON///
		/////////////////////////////////////////////////
		simon_top += 52;
		simon_bottom += 5;///để xét va chạm với bậc thang đầu tiên khi bước xuống
		simon_left += 10;
		simon_right += 10;
		for(UINT i =0;i<listStair->size();i++)
		{
			float stair_left, stair_right, stair_top, stair_bottom;
			listStair->at(i)->GetBBox(stair_left, stair_top, stair_right, stair_bottom);
			if (AABB(simon_left, simon_top, simon_right, simon_bottom, stair_left, stair_top, stair_right, stair_bottom) == true)
			{
				DebugOut(L"Đã vào");
				int status;
				listStair->at(i)->GetStatus(status);
				if (status == 0)
				{
					stairDirection = 1;
				}
				else
					stairDirection = -1;
				stairCollided = listStair->at(i);

				//bậc thang ở dưới so với chân simon->có thể di chuyển xuống
				if (simon_bottom < stair_bottom)
				{
					isMoveDown = true;
					return true;
				}
				if (stair_top - y <= 30)
				{
					isMoveUp = true;
					return true;
				}
				//kiểm tra xem simon có thể di chuyển lên hay không?
				//vì mảng liststairs gồm các bậc thang liền kề nhau, nên chỉ cần kiểm tra 2 bậc là đủ.
				float upstair_x = -999, upstair_y = -999;//toạ độ của bậc thang liền kề ở phía trên(nếu có)
				for(UINT j=0;j<listStair->size();j++)
				{
					if(i==j)
					{
						continue;
					}
					listStair->at(j)->GetPosition(upstair_x, upstair_y);
					float dx = abs(upstair_x - stair_left);
					float dy = upstair_y - stair_top;
					if(dx==32&&dy==-32)
					{
						isMoveUp = true;
					}
					if(dx==32 && dy==32)
					{
						isMoveDown = true;
					}
				}
				return true;
			}
		}
		isMoveDown = false;
		isMoveUp = false;
		return false;
	}
	void Simon::PositionCorrection(int prevState)
	{
		float stair_x, stair_y;
		stairCollided->GetPosition(stair_x, stair_y);
		if(prevState==-1)
		{
			if(status==STAIR_UP)
			{
				if(stairDirection==1)
				{
					x = stair_x - 44.0f;
					y = stair_y - 35.0f;
				}
				else
				{
					x = stair_x + 6.0f;
					y = stair_y - 31.0f;
				}
			}
			else if(status==STAIR_DOWN)
			{
				if(stairDirection==1)
				{
					x = stair_x - 15.0f;
					y = stair_y - 56.0f;
				}
				else
				{
					x = stair_x - 18.0f;
					y = stair_y - 47.0f;
				}

			}
		}
		else
		{
			if (status == STAIR_UP && prevState == STAIR_DOWN)
			{
				if (stairDirection == 1)
				{
					x -= 15.0f;
				}
				else
				{
					x += 3.0f;
				}
			}
			else if(status==STAIR_DOWN &&prevState==STAIR_UP)
			{
				if(stairDirection==1)
				{
					x += 15.0f;
				}
				else
				{
					x -= 3.0f;
				}
			}
		}
	}
	void Simon::StandOnStair()
	{
		vx = 0;
		vy = 0;
	}



}