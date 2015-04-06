#include "Main.h"
#include "UpdateManager.h"
#include "RenderManager.h"
#include <iostream>

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd )
{
	UpdateManager::Instance().initialize( appInstance );
	RenderManager::Instance().initialize( appInstance );

	bool running = true;
	while ( running )
	{
		UpdateManager::Instance().update(); // should not be called every single frame
		RenderManager::Instance().render();

		// InputManager::Instance().pollInput()
	}

	system( "pause" );
	return 0;
}