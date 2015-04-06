#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "Singleton.h"
#include "RenderManager.h"
#include "GameTimer.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define WINDOW_START_WIDTH  1200
#define WINDOW_START_HEIGHT 800
#define WINDOW_MIN_WIDTH	400
#define WINDOW_MIN_HEIGHT	300

static LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

class Window
{
	SINGLETON_INSTANCE( Window );
	friend LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
public:
	bool initialize( HINSTANCE appInstance, GameTimer** timer );
	void onResize( void );

	HWND getWindow( void ) const;

	u32 getWindowWidth( void ) const;
	u32 getWindowHeight( void ) const;
	f32 getAspectRatio( void ) const;

	bool isMinimized( void ) const;
	bool isMaximized( void ) const;
	bool isResizing( void ) const;
	bool isGamePaused( void ) const;
	
private:
	HINSTANCE	hInstance;
	HWND		hWindow;

	u32			windowWidth;
	u32			windowHeight;

	bool		minimized;
	bool		maximized;
	bool		resizing;
	bool		gamePaused;
	GameTimer	timer;

};

#endif