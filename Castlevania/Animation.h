#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprite.h"
#include "Game.h"
namespace core
{
	class CAnimationFrame
	{
		LPSPRITE sprite;
		DWORD time;
	public:
		CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }

		LPSPRITE GetSprite() { return this->sprite; }
		DWORD GetTime() { return this->time; }
	};
	typedef CAnimationFrame* LPANIMATION_FRAME;
	class CAnimation
	{
		DWORD animStartTime;
		DWORD lastFrameTime;
		int defaultTime;
		int currentFrame;
		vector<LPANIMATION_FRAME> frames;

	public:

		CAnimation(int defaultTime = 100);

		void SetAniStartTime(DWORD t) { animStartTime = t; }
		bool IsOver(DWORD dt) { return GetTickCount() - animStartTime >= dt; }
		void Reset() { currentFrame = -1; }

		int GetCurrentFrame() { return currentFrame; }
		int GetFramesSize() { return frames.size(); }

		void Add(int spriteID, DWORD time = 0);
		void Render(int RENDER_CAM, int nx, float x, float y, int alpha = 255);
		void RenderByID(int currentID, int nx, float x, float y, int alpha = 255);
	};

	typedef CAnimation* LPANIMATION;
	class CAnimations
	{
		static CAnimations* _instance;
		unordered_map<int, LPANIMATION> animations;

	public:
		void Add(int id, LPANIMATION ani) { animations[id] = ani; }
		LPANIMATION Get(int id) { return animations[id]; }

		static CAnimations* GetInstance();
	};
}