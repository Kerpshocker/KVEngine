#include "Main.h"
#include "Window.h"
#include "GameManager.h"
#include "RenderManager.h"
#include "MemoryManager.h"
#include "InputManager.h"
#include <iostream>

LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	default:
		return DefWindowProc( hwnd, msg, wParam, lParam );
	}
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

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = window->width();
	viewport.Height = window->height();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	MemoryManager::Instance().initialize();
	RenderManager::Instance().initialize( &viewport, 1 );
	RenderManager::Instance().setWindow( window );
	GameManager::Instance().initialize( window );
    SteelBattalion::InputManager();

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
				RenderManager::Instance().render();
                
			}
		}
	}

	release();

	return (int) msg.wParam;
}

void release( void )
{
	RenderManager::Instance().release();
	delete window;
}