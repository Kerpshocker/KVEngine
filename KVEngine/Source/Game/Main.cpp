#include "Main.h"
#include "Window.h"
#include "UpdateManager.h"
#include "RenderManager.h"
#include <iostream>

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd )
{
	Window::Instance().initialize( appInstance, &timer );
	UpdateManager::Instance().initialize();
	RenderManager::Instance().initialize();

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

			if ( Window::Instance().isGamePaused() )
			{
				Sleep( 100 );
			}
			else
			{
				UpdateManager::Instance().update();
				RenderManager::Instance().render();
			}
		}
	}

	return (int) msg.wParam;
}