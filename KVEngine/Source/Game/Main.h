#ifndef MAIN_H
#define MAIN_H

#include <Windows.h>
#include "GameTimer.h"
#include "DXWindow.h"

static DXWindow* window;
static GameTimer* timer;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd );

#endif