#include "UpdateManager.h"
#include "Window.h"

static LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
static WNDPROC MainWndProc = [] ( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) { return MsgProc( hwnd, msg, wParam, lParam ); };

void UpdateManager::initialize( void )
{
	bool pass = true;

	pass = Window::Instance().initialize( MainWndProc );
}

void UpdateManager::update( void )
{

}

LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}