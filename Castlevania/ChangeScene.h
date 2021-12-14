#pragma once
#include "Utils.h"
namespace static_object
{
	class ChangeScene
	{
		float x;
		float y;

		int ID_SCENE;
	public:
		ChangeScene(float x, float y, int ID_SCENE);
		void GetBBox(float& l, float& t, float& r, float& b);

		int Get_ID_SCENE() { return this->ID_SCENE; }
	};
	typedef ChangeScene* LPCHANGESCENEOBJ;
}

