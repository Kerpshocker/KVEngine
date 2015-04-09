#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "Singleton.h"
#include "RenderManager.h"
#include "GameTimer.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

//#define WINDOW_START_WIDTH  1200
//#define WINDOW_START_HEIGHT 800
//#define WINDOW_MIN_WIDTH	400
//#define WINDOW_MIN_HEIGHT	300
//
//static LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
//
//class Window
//{
//	SINGLETON_INSTANCE( Window );
//	friend LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
//public:
//	bool initialize( HINSTANCE appInstance, GameTimer** timer );
//	void onResize( void );
//
//	HWND getWindow( void ) const;
//
//	u32 getWindowWidth( void ) const;
//	u32 getWindowHeight( void ) const;
//	f32 getAspectRatio( void ) const;
//
//	bool isMinimized( void ) const;
//	bool isMaximized( void ) const;
//	bool isResizing( void ) const;
//	bool isGamePaused( void ) const;
//	
//private:
//	HINSTANCE	hInstance;
//	HWND		hWindow;
//
//	u32			windowWidth;
//	u32			windowHeight;
//
//	bool		minimized;
//	bool		maximized;
//	bool		resizing;
//	bool		gamePaused;
//	GameTimer	timer;
//
//};

typedef struct WindowParams
{
	HINSTANCE HInstance;
	u16 Width;
	u16 Height;
	LPCWSTR Name;
} WindowParams;

class Window
{
protected:
	Window( const WindowParams& params );

	void onResize( u16 width, u16 height );

	const u16 width( void ) const;
	const u16 height( void ) const;
	const f32 aspectRatio( void ) const;

protected:

	HWND		m_HWindow;

	u16			m_Width;
	u16			m_Height;
	f32			m_AspectRatio;

	bool		m_Minimized		: 1;
	bool		m_Maximized		: 1;
	bool		m_Resizing		: 1;
	bool		m_GamePaused	: 1;

};

#endif