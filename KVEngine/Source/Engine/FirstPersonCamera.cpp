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
			XMVECTOR forward = XMQuaternionMultiply( *m_Rotation, XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ) );
			forward = XMQuaternionMultiply( forward, XMQuaternionInverse( *m_Rotation ) );
			*m_Position += forward * amount;
		}

		void FirstPersonCamera::rotatePitch( float radians )
		{
			*m_Rotation = XMQuaternionMultiply( *m_Rotation, XMQuaternionRotationRollPitchYaw( -radians, 0.0f, 0.0f ) );
			*m_Rotation = XMQuaternionNormalize( *m_Rotation );
		}

		void FirstPersonCamera::rotateYaw( float radians )
		{
			*m_Rotation = XMQuaternionMultiply( *m_Rotation, XMQuaternionRotationRollPitchYaw( 0.0f, -radians, 0.0f ) );
			*m_Rotation = XMQuaternionNormalize( *m_Rotation );
		}

		void FirstPersonCamera::rotateRoll( float radians )
		{
			*m_Rotation = XMQuaternionMultiply( *m_Rotation, XMQuaternionRotationRollPitchYaw( 0.0f, 0.0f, radians ) );
			*m_Rotation = XMQuaternionNormalize( *m_Rotation );
		}
	}
}