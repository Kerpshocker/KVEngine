#include "Main.h"
#include "Window.h"
#include "GameManager.h"
#include "RenderManager.h"
#include "MemoryManager.h"
#include "InputManager.h"
#include "FrameManager.h"
#include "CameraManager.h"
#include <iostream>

using namespace KVE;

void RefreshScreen( void )
{
	window->resize();
	Graphics::CameraManager::Instance().getActiveCamera()->setProjMatrix( window->aspectRatio() );
	Graphics::RenderManager::Instance().setViewport( &window->getWindowViewport(), 0 );
}

LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_ACTIVATE:
		if ( window == nullptr ) return 0;
		if ( LOWORD( wParam ) == WA_INACTIVE )
		{
			window->setPaused( true );
			timer->stop();
		}
		else
		{
			window->setPaused( false );
			timer->start();
		}
		return 0;
	case WM_SIZE:
		if ( window == nullptr ) return 0;
		window->setWidth( LOWORD( lParam ) );
		window->setHeight( HIWORD( lParam ) );

		if ( wParam == SIZE_MINIMIZED )
		{
			window->setPaused( true );
			window->setMinimized( true );
			window->setMaximized( false );
		}
		else if ( wParam == SIZE_MAXIMIZED )
		{
			window->setPaused( false );
			window->setMinimized( false );
			window->setMaximized( true );
		}
		else if ( wParam == SIZE_RESTORED )
		{
			if ( window->isMinimized() )
			{
				window->setPaused( false );
				window->setMinimized( false );
				*refreshScreen = true;
			}
			else if ( window->isMaximized() )
			{
				window->setPaused( false );
				window->setMaximized( false );
				*refreshScreen = true;
			}
			else if ( window->isResizing() )
			{
				// do nothing until done
			}
			else
			{
				window->resize();
				RefreshScreen();
			}
		}

		return 0;
	case WM_ENTERSIZEMOVE:
		window->setPaused( true );
		window->setResizing( true );
		timer->stop();
		return 0;
	case WM_EXITSIZEMOVE:
		window->setPaused( false );
		window->setResizing( false );
		timer->start();
		*refreshScreen = true;
		return 0;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	case WM_MENUCHAR:
		return MAKELRESULT( 0, MNC_CLOSE );
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	case WM_KEYDOWN:
		std::cout << wParam << std::endl;
		Input::RegisterKeyPress( wParam );
		return 0;
	case WM_KEYUP:
		std::cout << wParam << std::endl;
		Input::RegisterKeyRelease( wParam );
		return 0;
	default:
		return DefWindowProc( hwnd, msg, wParam, lParam );
	}
}
static WNDPROC MainWndProc = []( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) { return MsgProc( hwnd, msg, wParam, lParam ); };

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd )
{
	System::WindowParams windowParams;
	windowParams.HInstance = appInstance;
	windowParams.Width = 800;
	windowParams.Height = 600;
	windowParams.Name = L"ASTEROIDS";
	windowParams.WndProcedure = MainWndProc;
	window = new Graphics::DXWindow( windowParams );

	timer = new System::GameTimer();

	refreshScreen = new std::atomic_bool();
	*refreshScreen = false;

#ifdef MULTI_THREADED
	running = new std::atomic_bool();
	running->store( true );
#endif

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = window->width();
	viewport.Height = window->height();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	System::MemoryManager::Instance().initialize();
	Graphics::FrameManager::Instance().initialize( 2 );
	Input::Initialize();
	Graphics::RenderManager::Instance().initialize( 1, &viewport, 1 );
	Graphics::RenderManager::Instance().setWindow( window );
	GameManager::Instance().initialize( window, timer );

	MSG msg = { 0 };
	timer->reset();

#ifdef MULTI_THREADED
	tGameLogic = std::thread( RunGameLogicThread );
	tRenderLogic = std::thread( RunRenderLogicThread );
#endif
	
	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			timer->tick();

			if ( window->isPaused() )
			{
				Sleep( 100 );
			}
			else
			{
#ifndef MULTI_THREADED
				if (*refreshScreen)	RefreshScreen();

				CalculateFrameStats();
				GameManager::Instance().update();
				Graphics::RenderManager::Instance().render();
#endif
			}
		}
	}

	Release();

	return (int) msg.wParam;
}

void Release( void )
{
#ifdef MULTI_THREADED
	running->store( false );
	tGameLogic.join();
	tRenderLogic.join();
#endif

	Graphics::RenderManager::Instance().release();
	System::MemoryManager::Instance().release();
	delete window;
	delete timer;
}

void CalculateFrameStats( void )
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ( ( timer->totalTime() - timeElapsed ) >= 1.0f )
	{
		float fps = (float) frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision( 6 );
		outs << "windowCaption" << L"    "
			<< L"Width: " << window->width() << L"    "
			<< L"Height: " << window->height() << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";

		SetWindowText( *window->getHandle(), outs.str().c_str() );

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

#ifdef MULTI_THREADED
void RunGameLogicThread( void )
{
	while ( running->load() )
	{
		if ( !window->isPaused() && KVE::Graphics::FrameManager::Instance().isWriteReady() )
		{
			GameManager::Instance().update();
			//CalculateFrameStats();
		}
	}
}

void RunRenderLogicThread( void )
{
	while ( running->load() )
	{
		if ( !window->isPaused() && KVE::Graphics::FrameManager::Instance().isReadReady() )
		{
			if (*refreshScreen)	RefreshScreen();

			Graphics::RenderManager::Instance().render();
			CalculateFrameStats();
		}
	}
}
#endif