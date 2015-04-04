#include "Camera.h"
#include <iostream>

namespace KVE
{
	Camera::Camera( const CameraParams& params )
	{
		fieldOfView = params.fieldOfView;
		nearPlane = params.nearPlane;
		farPlane = params.farPlane;
	}

	void Camera::debugPrint( void )
	{
		printf( "Camera:\n  fieldOfView=%f\n  nearPlane=%f\n  farPlane=%f\n", fieldOfView, nearPlane, farPlane );
	}
}