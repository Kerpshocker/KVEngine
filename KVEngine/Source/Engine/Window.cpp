#include "Window.h"
#include "DataTypes.h"

#define WINDOW_CLASS_NAME L"D3DWndClass"
#define WINDOW_CAPTION L"DirectX Game"

bool Window::initialize( HINSTANCE appInstance, WNDPROC mainWndProc )
{
	hInstance = appInstance;

	WNDCLASS window;
	window.style			= CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc		= mainWndProc;
	window.cbClsExtra		= 0;
	window.cbWndExtra		= 0;
	window.hInstance		= hInstance;
	window.hIcon			= LoadIcon( 0, IDI_APPLICATION );
	window.hCursor			= LoadCursor( 0, IDC_ARROW );
	window.hbrBackground	= (HBRUSH)GetStockObject( NULL_BRUSH );
	window.lpszMenuName		= 0;
	window.lpszClassName	= WINDOW_CLASS_NAME;
	if ( !RegisterClass( &window ) )
	{
		MessageBox( 0, L"RegisterClass(...) Failed.", 0, 0 );
		return false;
	}

	RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, false );
	i32 width = rect.right - rect.left;
	i32 height = rect.bottom - rect.top;

	hWindow = 0;
	hWindow = CreateWindowEx( 0, WINDOW_CLASS_NAME, WINDOW_CAPTION, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0 );
	if ( !hWindow )
	{
		MessageBox( 0, L"CreateWindow(...) failed.", 0, 0 );
		return false;
	}

	ShowWindow( hWindow, SW_SHOW );
	UpdateWindow( hWindow );

	return true;
}

void Window::onResize( void )
{
	RenderManager::Instance().onResize();
}

int Window::getWindowWidth( void ) const
{
	return windowWidth;
}

int Window::getWindowHeight( void ) const
{
	return windowHeight;
}

float Window::getAspectRatio( void ) const
{
	return (float)windowWidth / (float)windowHeight;
}