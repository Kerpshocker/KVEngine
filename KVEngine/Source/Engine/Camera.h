#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>
#include "DataTypes.h"

namespace KVE
{
	namespace Graphics
	{
		struct CameraParams
		{
			f32					fieldOfView;
			f32					nearPlane;
			f32					farPlane;
			DirectX::XMFLOAT3	initialPos;
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

		protected:
			DirectX::XMVECTOR position;
			DirectX::XMVECTOR rotation;
			DirectX::XMVECTOR scale;

		private:
			f32 fieldOfView;
			f32 nearPlane;
			f32 farPlane;

			DirectX::XMFLOAT4X4 projMatrix;
			DirectX::XMFLOAT4X4 viewMatrix;
		};
	}
}

#endif