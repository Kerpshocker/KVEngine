#include "UpdateManager.h"
#include "Window.h"

static LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
static WNDPROC MainWndProc = [] ( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) { return MsgProc( hwnd, msg, wParam, lParam ); };

void UpdateManager::initialize( HINSTANCE appInstance )
{
	bool pass = true;

	pass = Window::Instance().initialize( appInstance, MainWndProc );
}

void UpdateManager::update( void )
{

}

LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc( hwnd, msg, wParam, lParam );
}