#include "FirstPersonCamera.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace KVE
{
	namespace GameComponents
	{
		FirstPersonCamera::FirstPersonCamera( void ) { }

		void FirstPersonCamera::move( float amount )
		{
			XMVECTOR forward = XMQuaternionMultiply( rotation, XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ) );
			forward = XMQuaternionMultiply( forward, XMQuaternionInverse( rotation ) );
			position += forward * amount;
		}

		void FirstPersonCamera::rotatePitch( float radians )
		{
			rotation = XMQuaternionMultiply( rotation, XMQuaternionRotationRollPitchYaw( -radians, 0.0f, 0.0f ) );
			rotation = XMQuaternionNormalize( rotation );
		}

		void FirstPersonCamera::rotateYaw( float radians )
		{
			rotation = XMQuaternionMultiply( rotation, XMQuaternionRotationRollPitchYaw( 0.0f, -radians, 0.0f ) );
			rotation = XMQuaternionNormalize( rotation );
		}

		void FirstPersonCamera::rotateRoll( float radians )
		{
			rotation = XMQuaternionMultiply( rotation, XMQuaternionRotationRollPitchYaw( 0.0f, 0.0f, radians ) );
			rotation = XMQuaternionNormalize( rotation );
		}
	}
}