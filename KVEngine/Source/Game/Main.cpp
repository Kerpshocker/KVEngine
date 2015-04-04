#include "Main.h"
#include "Camera.h"
#include <iostream>

void main( void )
{
	KVE::CameraParams camParams = KVE::CameraParams
	{
		45.0f,
		0.00001f,
		100.0f
	};
	KVE::Camera camera = KVE::Camera( camParams );
	camera.debugPrint();

	system( "pause" );
}