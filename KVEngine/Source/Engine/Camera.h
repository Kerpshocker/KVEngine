#ifndef CAMERA_H
#define CAMERA_H

#include "DataTypes.h"

namespace KVE
{
	struct CameraParams
	{
		f32 fieldOfView;
		f32 nearPlane;
		f32 farPlane;
	};

	class Camera
	{
	public:
		Camera( const CameraParams& params );

		void debugPrint( void );

	private:
		f32 fieldOfView;
		f32 nearPlane;
		f32 farPlane;

		/* pseudo-code
		vector3* mountPosition;
		vector3* mountRotation;
		*/

	};
}

#endif