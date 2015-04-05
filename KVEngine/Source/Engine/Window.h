#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "Singleton.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

class Window
{
	SINGLETON_INSTANCE( Window );
public:
	bool initialize( WNDPROC mainWndProc );
	void onResize( void );

private:
	bool minimized;
	bool maximized;
	bool resizing;

};

#endif