#include "GameObject.h"
namespace core
{
	CGameObject::CGameObject()
	{
		x = 0;
		y = 0;
		status = 0;
		vx = 0;
		vy = 0;
		Orientation_x = -1;
		Is_Enable = true;
		Id_item_that_obj_carry = 1;
	}
	void CGameObject::RenderBBox()
	{
		LPDIRECT3DTEXTURE9 bbox = CTexture::GetInstance()->Get(ID_TEXTURE_BBOX);
		float l, t, r, b;
		RECT rect;
		GetBBox(l, t, r, b);
		rect.left = 0;
		rect.top = 0;
		rect.right = (int)r - (int)l;
		rect.bottom = (int)b - (int)t;
		CGame::GetInstance()->draw(1, Orientation_x, l, t, bbox, 0, 0, rect.right, rect.bottom, 32);
	}
	bool CGameObject:: AABB(float left_obj, float top_obj, float right_obj, float bottom_obj, float left_other, float top_other, float right_other, float bottom_other)
	{
		return left_obj < right_other && right_obj > left_other && top_obj < bottom_other && bottom_obj > top_other;
	}
	void CGameObject::SweptAABB(
		float ml, float mt, float mr, float mb,
		float dx, float dy,
		float sl, float st,
		float sr, float sb,
		float& t, float& nx, float& ny)
	{
		float dx_entry, dx_exit, tx_entry, tx_exit;
		float dy_entry, dy_exit, ty_entry, ty_exit;

		float t_entry;
		float t_exit;

		t = -1.0f;		// no collision
		nx = ny = 0;


		//	Broad-phase test

		float bl = dx > 0 ? ml : ml + dx;
		float bt = dy > 0 ? mt : mt + dy;
		float br = dx > 0 ? mr + dx : mr;
		float bb = dy > 0 ? mb + dy : mb;

		if (br < sl || bl > sr || bt > sb || bb < st)
			return;

		// moving object is not moving
		if (dx == 0 && dy == 0)
			return;

		if (dx > 0)
		{
			dx_entry = sl - mr;
			dx_exit = sr - ml;
		}
		else if (dx < 0)
		{
			dx_entry = sr - ml;
			dx_exit = sl - mr;
		}

		if (dy > 0)
		{
			dy_entry = st - mb;
			dy_exit = sb - mt;
		}
		else if (dy < 0)
		{
			dy_entry = sb - mt;
			dy_exit = st - mb;
		}

		if (dx == 0)
		{
			tx_entry = -99999999999;
			tx_exit = 99999999999;
		}
		else
		{
			tx_entry = dx_entry / dx;
			tx_exit = dx_exit / dx;
		}

		if (dy == 0)
		{
			ty_entry = -99999999999;
			ty_exit = 99999999999;
		}
		else {
			ty_entry = dy_entry / dy;
			ty_exit = dy_exit / dy;
		}


		if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

		t_entry = max(tx_entry, ty_entry);
		t_exit = min(tx_exit, ty_exit);

		if (t_entry > t_exit) return;

		t = t_entry;

		if (tx_entry > ty_entry)
		{
			ny = 0.0f;
			dx > 0 ? nx = -1.0f : nx = 1.0f;
		}
		else
		{
			nx = 0.0f;
			dy > 0 ? ny = -1.0f : ny = 1.0f;
		}
	}
	LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
	{
		float sl, st, sr, sb;		// static object bbox
		float ml, mt, mr, mb;		// moving object bbox
		float t, nx, ny;

		coO->GetBBox(sl, st, sr, sb);

		// deal with moving object: m speed = original m speed - collide object speed
		float svx, svy;
		coO->GetSpeed(svx, svy);

		float sdx = svx * dt;
		float sdy = svy * dt;

		float dx = this->dx - sdx;
		float dy = this->dy - sdy;

		GetBBox(ml, mt, mr, mb);

		SweptAABB(
			ml, mt, mr, mb,
			dx, dy,
			sl, st, sr, sb,
			t, nx, ny
		);

		CollisionEvent* e = new CollisionEvent(t, nx, ny, coO);
		return e;
	}
	void CGameObject::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}

		std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
	}
	void CGameObject::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny)
	{
		min_tx = 1.0f;
		min_ty = 1.0f;
		int min_ix = -1;
		int min_iy = -1;

		nx = 0.0f;
		ny = 0.0f;

		coEventsResult.clear();

		for (UINT i = 0; i < coEvents.size(); i++)
		{
			LPCOLLISIONEVENT c = coEvents[i];

			if (c->t < min_tx && c->nx != 0) {			// (thời gian va chạm), (nx != 0 có va chạm theo trục x)
				min_tx = c->t; nx = c->nx; min_ix = i;
			}

			if (c->t < min_ty && c->ny != 0) {
				min_ty = c->t; ny = c->ny; min_iy = i;
			}
		}

		if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
		if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
	}
	
	void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* List_Objects_In_Game, vector<LPGAMEOBJECT>* coObject)
	{
		this->dt = dt;
		dx = vx * dt;
		dy = vy * dt;
	}
}