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

		position = XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f );
		scale = XMVectorSet( 1.0f, 1.0f, 1.0f, 1.0f );
		orientation.forward = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
		orientation.up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		orientation.right = XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f );

		projMatrix = DirectX::XMFLOAT4X4();
		viewMatrix = DirectX::XMFLOAT4X4();

		setProjMatrix();
		setViewMatrix();
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

	void Camera::setProjMatrix( void )
	{
		assert( farPlane > nearPlane );

		XMStoreFloat4x4( &projMatrix, XMMatrixTranspose( XMMatrixPerspectiveFovLH(
			fieldOfView, Window::Instance().getAspectRatio(), nearPlane, farPlane ) ) );
	}

	void Camera::setViewMatrix( void )
	{
		XMVECTOR eye = position - orientation.forward;
		XMVECTOR focus = eye + orientation.forward;

		XMStoreFloat4x4( &viewMatrix, XMMatrixTranspose( XMMatrixLookAtLH(
			eye, focus, orientation.up ) ) );
	}
}