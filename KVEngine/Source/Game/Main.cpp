#include "Main.h"
#include "UpdateManager.h"
#include "RenderManager.h"
#include <iostream>

void main( void )
{
	UpdateManager::Instance().initialize();
	RenderManager::Instance().initialize();

	bool running = true;
	while ( running )
	{
		UpdateManager::Instance().update(); // should not be called every single frame
		RenderManager::Instance().render();

		// InputManager::Instance().pollInput()
	}

	system( "pause" );
}