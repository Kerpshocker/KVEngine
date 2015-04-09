#include "Main.h"
#include "Window.h"
#include "GameManager.h"
#include "RenderManager.h"
#include <iostream>

LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc( hwnd, msg, wParam, lParam );
}
static WNDPROC MainWndProc = []( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) { return MsgProc( hwnd, msg, wParam, lParam ); };

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd )
{
	WindowParams windowParams;
	windowParams.HInstance = appInstance;
	windowParams.Width = 800;
	windowParams.Height = 600;
	windowParams.Name = L"TESTING";
	windowParams.WndProcedure = MainWndProc;
	window = new DXWindow( windowParams );

	timer = new GameTimer();

	GameManager::Instance().initialize();
	RenderManager::Instance().initialize( window );

	MSG msg = { 0 };
	timer->reset();
	
	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			timer->tick();

			if ( window->isPaused() )
			{
				Sleep( 100 );
			}
			else
			{
				GameManager::Instance().update();
				RenderManager::Instance().renderTo( window );
			}
		}
	}

	return (int) msg.wParam;
}