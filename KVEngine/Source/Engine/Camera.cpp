#include "Camera.h"
#include "Window.h"
#include <iostream>

namespace KVE
{
	using namespace DirectX;

	Camera::Camera( void )
	{

	}

	Camera::Camera( const CameraParams& params )
	{
		fieldOfView = params.fieldOfView;
		nearPlane = params.nearPlane;
		farPlane = params.farPlane;

		position = XMLoadFloat3( &params.initialPos );
		rotation = XMQuaternionIdentity();
		scale = XMVectorSet( 1.0f, 1.0f, 1.0f, 1.0f );

		projMatrix = DirectX::XMFLOAT4X4();
		viewMatrix = DirectX::XMFLOAT4X4();

		/*setProjMatrix( 0 );
		setViewMatrix();*/
	}

	void Camera::move( float amount )
	{
		XMVECTOR forward = XMQuaternionMultiply( rotation, XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ) );
		forward = XMQuaternionMultiply( forward, XMQuaternionInverse( rotation ) );
		position += forward * amount;
	}

	void Camera::rotatePitch( float radians )
	{
		rotation = XMQuaternionMultiply( rotation, XMQuaternionRotationRollPitchYaw( -radians, 0.0f, 0.0f ) );
		rotation = XMQuaternionNormalize( rotation );
	}

	void Camera::rotateYaw( float radians )
	{
		rotation = XMQuaternionMultiply( rotation, XMQuaternionRotationRollPitchYaw( 0.0f, -radians, 0.0f ) );
		rotation = XMQuaternionNormalize( rotation );
	}

	void Camera::rotateRoll( float radians )
	{
		rotation = XMQuaternionMultiply( rotation, XMQuaternionRotationRollPitchYaw( 0.0f, 0.0f, radians ) );
		rotation = XMQuaternionNormalize( rotation );
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
		XMVECTOR inverse = XMQuaternionInverse( rotation );

		XMVECTOR forward = XMQuaternionMultiply( rotation, XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ) );
		forward = XMQuaternionMultiply( forward, inverse );
		XMVECTOR up = XMQuaternionMultiply( rotation, XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) );
		up = XMQuaternionMultiply( up, inverse );

		XMStoreFloat4x4( &viewMatrix, XMMatrixTranspose( XMMatrixLookAtLH(
			position, position + forward, up ) ) );
	}
}