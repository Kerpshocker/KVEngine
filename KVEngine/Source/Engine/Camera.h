#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>
#include "DataTypes.h"

namespace KVE
{
	struct Orientation
	{
		DirectX::XMVECTOR forward;
		DirectX::XMVECTOR up;
		DirectX::XMVECTOR right;
	};

	struct CameraParams
	{
		f32 fieldOfView;
		f32 nearPlane;
		f32 farPlane;
	};

	class Camera
	{
	public:
		Camera( void );
		Camera( const CameraParams& params );

		const DirectX::XMFLOAT4X4 getProjMatrix( void )const;
		const DirectX::XMFLOAT4X4 getViewMatrix( void )const;

		void setProjMatrix( f32 aspectRatio );
		void setViewMatrix( void );
		void debugPrint( void );

	private:
		f32 fieldOfView;
		f32 nearPlane;
		f32 farPlane;

		DirectX::XMVECTOR position;
		Orientation orientation;
		DirectX::XMVECTOR scale;

		DirectX::XMFLOAT4X4 projMatrix;
		DirectX::XMFLOAT4X4 viewMatrix;
	};
}

#endif