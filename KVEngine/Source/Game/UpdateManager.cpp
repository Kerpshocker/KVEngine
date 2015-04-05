#include "UpdateManager.h"
#include "Window.h"

void UpdateManager::initialize( HINSTANCE appInstance )
{
	bool pass = true;

	pass = Window::Instance().initialize( appInstance );

	Manager::initialize();
}

void UpdateManager::update( void )
{

}