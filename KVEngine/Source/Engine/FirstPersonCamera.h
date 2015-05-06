#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "Camera.h"

namespace KVE
{
	namespace GameComponents
	{
		class FirstPersonCamera : public Graphics::Camera
		{
		public:
			FirstPersonCamera( void );

			void move( float amount );
			void rotatePitch( float radians );
			void rotateYaw( float radians );
			void rotateRoll( float radians );
		};
	}
}

#endif