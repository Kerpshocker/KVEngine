#include "Window.h"

#define WINDOW_CLASS_NAME L"D3DWndClass"
#define WINDOW_CAPTION L"DirectX Game"

static WNDPROC MainWndProc = []( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) { return MsgProc( hwnd, msg, wParam, lParam ); };

bool Window::initialize( HINSTANCE appInstance, GameTimer** timer )
{
	hInstance = appInstance;
	*timer = &this->timer;

	WNDCLASS window;
	window.style			= CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc		= MainWndProc;
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

	RECT rect = { 0, 0, WINDOW_START_WIDTH, WINDOW_START_HEIGHT };
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

HWND Window::getWindow( void ) const
{
	return hWindow;
}

u32 Window::getWindowWidth( void ) const
{
	return windowWidth;
}

u32 Window::getWindowHeight( void ) const
{
	return windowHeight;
}

f32 Window::getAspectRatio( void ) const
{
	return (float)windowWidth / (float)windowHeight;
}

bool Window::isMinimized( void ) const
{
	return minimized;
}

bool Window::isMaximized( void ) const
{
	return maximized;
}

bool Window::isResizing( void ) const
{
	return resizing;
}

bool Window::isGamePaused( void ) const
{
	return gamePaused;
}

LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	// sent when the window is activated or deactivated
	case WM_ACTIVATE:
		Window::Instance().gamePaused = ( LOWORD( wParam ) == WA_INACTIVE );
		Window::Instance().gamePaused ?
			Window::Instance().timer.stop() :
			Window::Instance().timer.start();
		return 0;
	// sent when the user resizes the window
	case WM_SIZE:
		Window::Instance().windowWidth  = LOWORD( lParam );
		Window::Instance().windowHeight = HIWORD( lParam );
		if ( !RenderManager::Instance().isInitialized() )
			return 0;
		if ( wParam == SIZE_MINIMIZED )
		{
			Window::Instance().gamePaused = true;
			Window::Instance().minimized = true;
			Window::Instance().maximized = false;
		}
		else if ( wParam == SIZE_MAXIMIZED )
		{
			Window::Instance().gamePaused = false;
			Window::Instance().minimized = false;
			Window::Instance().maximized = true;
		}
		else if ( wParam == SIZE_RESTORED )
		{
			if ( Window::Instance().minimized )
			{
				Window::Instance().gamePaused = false;
				Window::Instance().minimized = false;
				Window::Instance().onResize();
			}
			else if ( Window::Instance().maximized )
			{
				Window::Instance().gamePaused = false;
				Window::Instance().maximized = false;
				Window::Instance().onResize();
			}
			else if ( Window::Instance().resizing )
			{
				// do nothing while resizing, only when resizing is complete
			}
			else
			{
				Window::Instance().onResize();
			}
		}
		return 0;
	// sent when the user grabs the resize bars
	case WM_ENTERSIZEMOVE:
		Window::Instance().gamePaused = true;
		Window::Instance().resizing = true;
		Window::Instance().timer.stop();
		return 0;
	// sent when the user releases the resize bars
	case WM_EXITSIZEMOVE:
		Window::Instance().gamePaused = false;
		Window::Instance().resizing = false;
		Window::Instance().timer.start();
		Window::Instance().onResize();
		return 0;
	// sent when the window is being destroyed
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	// sent when a menu is active and user presses key not
	//	corresponding to any mnemonic or accelerator key
	case WM_MENUCHAR:
		return MAKELRESULT( 0, MNC_CLOSE );
	// catch this message to prevent the windoe from becoing too small
	case WM_GETMINMAXINFO:
		( (MINMAXINFO*)lParam )->ptMinTrackSize.x = WINDOW_MIN_WIDTH;
		( (MINMAXINFO*)lParam )->ptMinTrackSize.y = WINDOW_MIN_HEIGHT;
		return 0;
	
	// CATCH REST OF BUTTON INPUT BELOW HERE
	}

	return DefWindowProc( hwnd, msg, wParam, lParam );
}