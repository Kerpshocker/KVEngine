#include "Camera.h"
#include "Window.h"
#include <iostream>

using namespace DirectX;

namespace KVE
{
	namespace Graphics
	{
		Camera::Camera( void ) { }

		Camera::Camera( const CameraParams& params )
		{
			fieldOfView = params.fieldOfView;
			nearPlane = params.nearPlane;
			farPlane = params.farPlane;

			*m_Position = XMLoadFloat3( &params.initialPos );
			*m_Rotation = DirectX::XMQuaternionIdentity();
			*m_Scale = XMVectorSet( 1.0f, 1.0f, 1.0f, 1.0f );

			projMatrix = DirectX::XMFLOAT4X4();
			viewMatrix = DirectX::XMFLOAT4X4();
		}

		const XMFLOAT4X4 Camera::getProjMatrix( void ) const
		{
			return projMatrix;
		}

		const XMFLOAT4X4 Camera::getViewMatrix( void ) const
		{
			return viewMatrix;
		}

		void Camera::debugPrint( void )
		{
			printf( "Camera:\n  fieldOfView=%f\n  nearPlane=%f\n  farPlane=%f\n", fieldOfView, nearPlane, farPlane );
		}

		void Camera::setProjMatrix( f32 aspectRatio )
		{
			assert( farPlane > nearPlane );

			XMStoreFloat4x4( &projMatrix, XMMatrixTranspose( XMMatrixPerspectiveFovLH(
				fieldOfView, aspectRatio, nearPlane, farPlane ) ) );
		}

		void Camera::setViewMatrix( void )
		{
			XMVECTOR inverse = XMQuaternionInverse( *m_Rotation );

			XMVECTOR forward = XMQuaternionMultiply( *m_Rotation, XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ) );
			forward = XMQuaternionMultiply( forward, inverse );
			XMVECTOR up = XMQuaternionMultiply( *m_Rotation, XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) );
			up = XMQuaternionMultiply( up, inverse );

			XMStoreFloat4x4( &viewMatrix, XMMatrixTranspose( XMMatrixLookAtLH(
				*m_Position, *m_Position + forward, up ) ) );
		}
	}
}