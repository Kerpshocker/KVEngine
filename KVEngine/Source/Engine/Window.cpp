#include "Window.h"

Window::Window( const WindowParams& params )
{
	onResize( params.Width, params.Height );

	WNDCLASS window;
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = params.WndProcedure;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hInstance = params.HInstance;
	window.hIcon = LoadIcon( 0, IDI_APPLICATION );
	window.hCursor = LoadCursor( 0, IDC_ARROW );
	window.hbrBackground = (HBRUSH) GetStockObject( NULL_BRUSH );
	window.lpszMenuName = 0;
	window.lpszClassName = params.Name;
	if ( !RegisterClass( &window ) )
	{
		MessageBox( 0, L"RegisterClass(...) Failed", 0, 0 );
		return;
	}

	RECT rect = { 0, 0, m_Width, m_Height };
	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, true );
	long rWidth = rect.right - rect.left;
	long rHeight = rect.bottom - rect.top;

	m_HWindow = 0;
	m_HWindow = CreateWindowEx( 0, params.Name, L"Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rWidth, rHeight, 0, 0, params.HInstance, 0 );
	if ( !m_HWindow )
	{
		MessageBox( 0, L"CreateWindow(...) failed.", 0, 0 );
		return;
	}

	ShowWindow( m_HWindow, SW_SHOW );
	UpdateWindow( m_HWindow );
}

void Window::onResize( u16 width, u16 height )
{
	m_Width = width;
	m_Height = height;
	m_AspectRatio = (f32) m_Width / (f32) m_Height;
}

const u16 Window::width( void ) const
{
	return m_Width;
}

const u16 Window::height( void ) const
{
	return m_Height;
}

const f32 Window::aspectRatio( void ) const
{
	return m_AspectRatio;
}

const bool Window::isMinimized( void ) const
{
	return m_Minimized;
}

const bool Window::isMaximized( void ) const
{
	return m_Maximized;
}

const bool Window::isResizing( void ) const
{
	return m_Resizing;
}

const bool Window::isPaused( void ) const
{
	return m_Paused;
}


//LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
//{
//	switch ( msg )
//	{
//	// sent when the window is activated or deactivated
//	case WM_ACTIVATE:
//		Window::Instance().gamePaused = ( LOWORD( wParam ) == WA_INACTIVE );
//		Window::Instance().gamePaused ?
//			Window::Instance().timer.stop() :
//			Window::Instance().timer.start();
//		return 0;
//	// sent when the user resizes the window
//	case WM_SIZE:
//		Window::Instance().windowWidth  = LOWORD( lParam );
//		Window::Instance().windowHeight = HIWORD( lParam );
//		if ( !RenderManager::Instance().isInitialized() )
//			return 0;
//		if ( wParam == SIZE_MINIMIZED )
//		{
//			Window::Instance().gamePaused = true;
//			Window::Instance().minimized = true;
//			Window::Instance().maximized = false;
//		}
//		else if ( wParam == SIZE_MAXIMIZED )
//		{
//			Window::Instance().gamePaused = false;
//			Window::Instance().minimized = false;
//			Window::Instance().maximized = true;
//		}
//		else if ( wParam == SIZE_RESTORED )
//		{
//			if ( Window::Instance().minimized )
//			{
//				Window::Instance().gamePaused = false;
//				Window::Instance().minimized = false;
//				Window::Instance().onResize();
//			}
//			else if ( Window::Instance().maximized )
//			{
//				Window::Instance().gamePaused = false;
//				Window::Instance().maximized = false;
//				Window::Instance().onResize();
//			}
//			else if ( Window::Instance().resizing )
//			{
//				// do nothing while resizing, only when resizing is complete
//			}
//			else
//			{
//				Window::Instance().onResize();
//			}
//		}
//		return 0;
//	// sent when the user grabs the resize bars
//	case WM_ENTERSIZEMOVE:
//		Window::Instance().gamePaused = true;
//		Window::Instance().resizing = true;
//		Window::Instance().timer.stop();
//		return 0;
//	// sent when the user releases the resize bars
//	case WM_EXITSIZEMOVE:
//		Window::Instance().gamePaused = false;
//		Window::Instance().resizing = false;
//		Window::Instance().timer.start();
//		Window::Instance().onResize();
//		return 0;
//	// sent when the window is being destroyed
//	case WM_DESTROY:
//		PostQuitMessage( 0 );
//		return 0;
//	// sent when a menu is active and user presses key not
//	//	corresponding to any mnemonic or accelerator key
//	case WM_MENUCHAR:
//		return MAKELRESULT( 0, MNC_CLOSE );
//	// catch this message to prevent the windoe from becoing too small
//	case WM_GETMINMAXINFO:
//		( (MINMAXINFO*)lParam )->ptMinTrackSize.x = WINDOW_MIN_WIDTH;
//		( (MINMAXINFO*)lParam )->ptMinTrackSize.y = WINDOW_MIN_HEIGHT;
//		return 0;
//	
//	// CATCH REST OF BUTTON INPUT BELOW HERE
//	}
//
//	return DefWindowProc( hwnd, msg, wParam, lParam );
//}