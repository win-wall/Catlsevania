#include "ChangeScene.h"
namespace static_object
{
	ChangeScene::ChangeScene(float x, float y, int ID_SCENE)
	{
		this->x = x;
		this->y = y;
		this->ID_SCENE = ID_SCENE;
	}
	void ChangeScene::GetBBox(float& l, float& t, float& r, float& b)
	{
		l = x;
		t = y;
		r = l + 32;
		b = t + 32;
	}
}