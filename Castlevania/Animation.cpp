#include "Animation.h"

namespace core
{
	CAnimation::CAnimation(int defaultTime)
	{
		this->defaultTime = defaultTime;
		lastFrameTime = -1;
		currentFrame = -1;
	}

	void CAnimation::Add(int spriteID, DWORD time)
	{
		if (time == 0) time = this->defaultTime;

		LPSPRITE sprite = CSprites::GetInstance()->Get(spriteID);
		LPANIMATION_FRAME frame = new CAnimationFrame(sprite, time);
		frames.push_back(frame);
	}

	void CAnimation::Render(int RENDER_CAM, int nx, float x, float y, int alpha)
	{
		DWORD now = GetTickCount();

		if (currentFrame == -1)
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
		else
		{
			DWORD t = frames[currentFrame]->GetTime();
			if (now - lastFrameTime >= t) {
				currentFrame++;
				lastFrameTime = now;

				if (currentFrame >= frames.size())
				{
					currentFrame = 0;
				}
			}
		}

		frames[currentFrame]->GetSprite()->Draw(RENDER_CAM, nx, x, y, alpha);
	}

	void CAnimation::RenderByID(int currentID, int nx, float x, float y, int alpha)
	{
		if (frames.size() == 3)
		{
			frames[currentID]->GetSprite()->Draw(1, nx, x, y, alpha);
		}
		else
		{
			int rd = rand() % 4;

			frames[currentID * 4 + rd]->GetSprite()->Draw(1, nx, x, y, alpha);
		}
	}
	CAnimations* CAnimations::_instance = NULL;

	CAnimations* CAnimations::GetInstance()
	{
		if (_instance == NULL) _instance = new CAnimations();
		return _instance;
	}
}