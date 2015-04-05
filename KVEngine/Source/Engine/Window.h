#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "Singleton.h"
#include "RenderManager.h"
#include "GameTimer.h"

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
	bool initialize( HINSTANCE appInstance );
	void onResize( void );

	int getWindowWidth( void ) const;
	int getWindowHeight( void ) const;
	float getAspectRatio( void ) const;

private:
	HINSTANCE	hInstance;
	HWND		hWindow;

	int			windowWidth;
	int			windowHeight;

	bool		minimized;
	bool		maximized;
	bool		resizing;
	bool		gamePaused;
	GameTimer	timer;

};

#endif