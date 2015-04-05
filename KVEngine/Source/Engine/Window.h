#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "Singleton.h"
#include "RenderManager.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

class Window
{
	SINGLETON_INSTANCE( Window );
public:
	bool initialize( HINSTANCE appInstance, WNDPROC mainWndProc );
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

};

#endif