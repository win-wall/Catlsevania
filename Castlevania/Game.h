#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>


#include "Utils.h"
#include "KeyEventHandler.h"
#include "Texture.h"

namespace core
{
	class CTexture;
	class CAnimations;
	class CGame
	{
		static CGame* __instance;

		HWND hWnd;									// Window handle

		LPDIRECT3D9 d3d = NULL;						// Direct3D handle
		LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

		LPDIRECT3DSURFACE9 backBuffer = NULL;
		LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

		LPDIRECTINPUT8       di;		// The DirectInput object         
		LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 
		BYTE  keyStates[256];			// DirectInput keyboard state buffer 
		DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data
		input::LPKEYEVENTHANDLER keyHandler;

		ID3DXFont* font;

		float cam_x = 0.0f;
		float cam_y = 0.0f;

		
	public:
		//////////////////////
		///////KEYBROAD///////
		//////////////////////
		void InitKeyboard(input::LPKEYEVENTHANDLER handler);
		int IsKeyDown(int KeyCode);
		void ProcessKeyboard();

		/////////////////////
		/////////CGAME///////
		/////////////////////
		void Init(HWND hWnd);
		void draw(int CAM_RENDER, int orientation, float position_x, float position_y, LPDIRECT3DTEXTURE9 texture,
			int left,
			int top,
			int right,
			int bottom,
			int alpha=255);
		//Render theo camera hay không? 1:có 2:không

		/////////////////////
		////////GET//////////
		/////////////////////
		LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
		LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
		LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
		static CGame* GetInstance();

		///////////////////////
		/////////GET CAMERA////
		///////////////////////
		void GetCameraPositon(float& camera_x, float& camera_y) { camera_x = this->cam_x; camera_y = this->cam_y; }
		
		///////////////////////
		/////////SET CAMERA////
		///////////////////////
		void SetCamaraPosition(float new_position_cam_x, float new_position_cam_y);

		///////////////////////
		///////DETRUCTOR//////
		//////////////////////
		~CGame();
		friend class CTexture;
		
	};
}

