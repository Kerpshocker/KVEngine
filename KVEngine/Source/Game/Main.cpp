#include "Main.h"
#include "Window.h"
#include "GameManager.h"
#include "RenderManager.h"
#include "MemoryManager.h"
#include "InputManager.h"
#include "FrameManager.h"
#include <iostream>

using namespace KVE;

LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
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
	Graphics::FrameManager::Instance().initialize();
	Input::Initialize();
	Graphics::RenderManager::Instance().initialize( &viewport, 1 );
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
		if ( !window->isPaused() )
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
		if ( !window->isPaused() )
		{
			Graphics::RenderManager::Instance().render();
			CalculateFrameStats();
		}
	}
}
#endif