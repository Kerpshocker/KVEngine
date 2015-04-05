#include "Window.h"

bool Window::initialize( WNDPROC mainWndProc )
{
	WNDCLASS window;
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = mainWndProc;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hIcon = LoadIcon( 0, IDI_APPLICATION );
	window.hCursor = LoadCursor( 0, IDC_ARROW );
	window.hbrBackground = (HBRUSH) GetStockObject( NULL_BRUSH );
	window.lpszMenuName = 0;
	window.lpszClassName = L"D3DWndClassName";

	if ( !RegisterClass( &window ) )
	{
		MessageBox( 0, L"RegisterClass Failed.", 0, 0 );
		return false;
	}
}