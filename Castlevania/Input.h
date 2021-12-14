#pragma once

#include "KeyEventHandler.h"
#include "Game.h"
#include "SceneManager.h"

namespace input
{
	class Input : public CKeyEventHandler
	{
		bool Is_Delay_Change_Sate = true; 
		core::CGame* game;
		core::SceneManager* scenes;
	public:
		Input(core::CGame* game, core::SceneManager* scenes);
		~Input();

		bool Animation_Standby();
		bool CanProcessKeyboard();
		virtual void KeyState(BYTE* state);
		virtual void OnKeyDown(int KeyCode);
		virtual void OnKeyUp(int KeyCode);
	};
}

