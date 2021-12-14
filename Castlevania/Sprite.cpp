#include "Sprite.h"
namespace core
{
	CSprite::CSprite(int SpriteID, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
	{
		this->SpriteID = SpriteID;
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
		this->texture = tex;
	}
	void CSprite::Draw(int RENDER_CAM, int nx, float x, float y, int alpha)
	{
		CGame* game = CGame::GetInstance();
		game->draw(RENDER_CAM, nx, x, y, texture, left, top, right, bottom, alpha);
	}
	CSprites* CSprites::_instance = NULL;
	void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
	{
		LPSPRITE sprite = new CSprite(id, left, top, right, bottom, tex);
		sprites[id] = sprite;
	}

	CSprites* CSprites::GetInstance()
	{
		if (_instance == NULL) _instance = new CSprites();
		return _instance;
	}
}
