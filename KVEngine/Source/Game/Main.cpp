#include "Main.h"
#include "Window.h"
#include "UpdateManager.h"
#include "RenderManager.h"
#include <iostream>

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd )
{
	WindowParams windowParams;
	windowParams.HInstance = appInstance;
	windowParams.Width = 800;
	windowParams.Height = 600;
	windowParams.Name = L"TESTING";
	window = new DXWindow( windowParams );

	timer = new GameTimer();

	//Window::Instance().initialize( appInstance, &timer );
	UpdateManager::Instance().initialize();
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

			//if ( Window::Instance().isGamePaused() )
			//{
			//	Sleep( 100 );
			//}
			//else
			//{
				UpdateManager::Instance().update();
				RenderManager::Instance().renderTo( window );
			//}
		}
	}

	return (int) msg.wParam;
}