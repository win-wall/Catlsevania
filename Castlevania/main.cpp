#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Utils.h"
#include "Game.h"
#include "SceneManager.h"
#include "Input.h"
#include "HUD.h"

core::SceneManager* scenes;
core::CGame* cgame;
input::Input* cinput;
HUD::HUD* hud;
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;

	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MAIN_WINDOW_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			MAIN_WINDOW_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW | WS_EX_TOPMOST | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);
	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}
void Update(DWORD dt)
{
	hud->Update(dt);
	scenes->Update(dt);
}
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = cgame->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = cgame->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = cgame->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{

		d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		scenes->Render();
		hud->Render();
		spriteHandler->End();
		d3ddv->EndScene();
	}
	d3ddv->Present(NULL, NULL, NULL, NULL);
}
int Run()
{
	MSG msg;
	int done = 0;
	DWORD gamePlayStartTime = GetTickCount();
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		DWORD now = GetTickCount();
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			cgame->ProcessKeyboard();
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	cgame = core::CGame::GetInstance();
	cgame->Init(hWnd);
	scenes = new core::SceneManager(cgame,SCENE_1);
	scenes->LoadResources();
	scenes->Init(SCENE_1);
	cinput = new input::Input(cgame, scenes);
	cgame->InitKeyboard(cinput);

	hud = new HUD::HUD(scenes, cgame);
	hud->Init();
	Run();
	return 0;
}