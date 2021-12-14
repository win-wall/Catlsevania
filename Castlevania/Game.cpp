#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream> 

#include "Sprite.h"
#include "Animation.h"
using namespace std;

namespace core
{
	CGame* CGame::__instance = NULL;
	
	////////////////////////
	/////////CGAME//////////
	////////////////////////
	//not don't (missing font)
	void CGame::Init(HWND hwnd)
	{
		LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

		this->hWnd = hwnd;

		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		RECT rect;
		GetClientRect(hWnd, &rect);

		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferHeight = rect.bottom + 1;
		d3dpp.BackBufferWidth = rect.right + 1;

		d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&d3ddv);

		if (d3ddv == NULL)
		{
			DebugOut(L"[ERROR] CreateDevice failed\n");
			return;
		}

		d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
		D3DXCreateSprite(d3ddv, &spriteHandler);
		
		OutputDebugString(L"[INFOMATION]:		InitGame done;\n");
	}
	//done
	void CGame::draw(int CAM_RENDER, int orientation, float position_x, float position_y, LPDIRECT3DTEXTURE9 texture,
		int left,
		int top,
		int right,
		int bottom,
		int alpha)
	{
		D3DXVECTOR3 position(position_x - cam_x * CAM_RENDER, position_y - cam_y * CAM_RENDER, 0);

		RECT rect;
		rect.left = left;
		rect.top = top;
		rect.right = right;
		rect.bottom = bottom;

		//Ma trận lưu phép transform trước và sau của sprite
		D3DXMATRIX Previous_Transform;
		D3DXMATRIX Current_Transform;

		spriteHandler->GetTransform(&Previous_Transform);

		//tâm transform center của sprite
		D3DXVECTOR2 center = D3DXVECTOR2(position.x + (right - left) / 2, position.y + (bottom - top) / 2);
		//orientation là hướng của object hiện tại
		//-1 là bên trái. thì scale là 1 không thay đổi
		//1 là bên phải. thì scale là -1 flip về hướng ngược lại
		D3DXVECTOR2 scale = D3DXVECTOR2(orientation > 0 ? -1 : 1, 1);

		//gán vào Current_Transform thông tin biến đổi được
		D3DXMatrixTransformation2D(&Current_Transform, &center, 0.0f, &scale, NULL, 0.0f, NULL);

		//Ma trận Final_Transform là ma trận biến đổi cuối cùng bằng cách nhân ma tran mới và cũ transform
		D3DXMATRIX Final_Transform = Current_Transform * Previous_Transform;
		spriteHandler->SetTransform(&Final_Transform);

		spriteHandler->Draw(texture, &rect, NULL, &position, D3DCOLOR_ARGB(alpha, 255, 255, 255));

		//Gán Final_Transform vào Previous_Transform
		spriteHandler->SetTransform(&Previous_Transform);
	}
	CGame* CGame::GetInstance()
	{
		if (__instance == NULL) __instance = new CGame();
		return __instance;
	}
	//////////////////////////
	///////////CAMERA/////////
	//////////////////////////
	void CGame::SetCamaraPosition(float new_position_cam_x, float new_position_cam_y)
	{
		this->cam_x = new_position_cam_x;
		this->cam_y = new_position_cam_y;
	}

	//////////////////////////
	//////////KEYBROAD////////
	//////////////////////////
	void CGame::InitKeyboard(input::LPKEYEVENTHANDLER handler)
	{
		HRESULT hr = DirectInput8Create(
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

		if (hr != DI_OK)
		{
			DebugOut(L"[ERROR]:		DirectInput8Create failed\n");
			return;
		}

		hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

		if (hr != DI_OK)
		{
			DebugOut(L"[ERROR]:		CreateDevice failed\n");
			return;
		}

		// Set the data format to "keyboard format" - a predefined data format

		hr = didv->SetDataFormat(&c_dfDIKeyboard);

		hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
		//
		// DirectInput uses unbuffered I/O (buffer size = 0) by default.
		// If you want to read buffered data, you need to set a nonzero
		// buffer size.
		//
		// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
		//
		// The buffer size is a DWORD property associated with the device.

		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

		hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

		hr = didv->Acquire();
		if (hr != DI_OK)
		{
			DebugOut(L"[ERROR]:		DINPUT8::Acquire failed\n");
			return;
		}

		this->keyHandler = handler;

		DebugOut(L"[INFOMATION]:		Keyboard has been initialized successfully\n");
	}
	int CGame::IsKeyDown(int KeyCode)
	{
		return (keyStates[KeyCode] & 0x80) > 0;
	}
	void CGame::ProcessKeyboard()
	{
		HRESULT hr;

		// First, collect all key states
		hr = didv->GetDeviceState(sizeof(keyStates), keyStates);

		if (FAILED(hr))
		{
			// If the keyboard lost focus or was not acquired then try to get control back 
			if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
			{
				HRESULT h = didv->Acquire();
				if (h == DI_OK)
				{
					DebugOut(L"[INFO] Keyboard re-acquired\n");
				}
				else return;
			}
			else
			{
				//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed\n");
				return;
			}
		}


		keyHandler->KeyState((BYTE*)&keyStates);


		// Collect all buffered events
		DWORD dwElements = KEYBOARD_BUFFER_SIZE;
		hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);

		if (FAILED(hr))
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed\n");
			return;
		}

		// Scan through all buffered events, check if the key is pressed or released
		for (DWORD i = 0; i < dwElements; i++)
		{
			int keyCode = keyEvents[i].dwOfs;
			int keyState = keyEvents[i].dwData;

			if ((keyState & 0x80) > 0)
				keyHandler->OnKeyDown(keyCode);
			else
				keyHandler->OnKeyUp(keyCode);
		}
	}
	///////////////////////////
	/////////DESTRUCTOR////////
	///////////////////////////
	CGame::~CGame()
	{
		if (spriteHandler != NULL) spriteHandler->Release();
		if (backBuffer != NULL) backBuffer->Release();
		if (d3ddv != NULL) d3ddv->Release();
		if (d3d != NULL) d3d->Release();
	}

}
