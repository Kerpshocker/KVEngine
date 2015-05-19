#ifndef MAIN_H
#define MAIN_H

#include <Windows.h>
#include <thread>
#include <atomic>
#include "GameTimer.h"
#include "DXWindow.h"

#define MULTI_THREADED

static KVE::Graphics::DXWindow* window;
static KVE::System::GameTimer* timer;
static std::atomic_bool* refreshScreen;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd );
void Release( void );
void CalculateFrameStats( void );

#ifdef MULTI_THREADED
static std::atomic_bool* running;
static std::thread tGameLogic;
static std::thread tRenderLogic;

void RunGameLogicThread( void );
void RunRenderLogicThread( void );
#endif

#endif