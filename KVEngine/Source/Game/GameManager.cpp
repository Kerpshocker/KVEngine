#include "GameManager.h"
#include "CameraManager.h"

void GameManager::initialize( void )
{
	KVE::CameraParams cParams;
	cParams.fieldOfView = 45.0f * ( 3.1415f / 180.0f );
	cParams.nearPlane = .01f;
	cParams.farPlane = 100.0f;

	CameraManager::Instance().createNewCamera( cParams, true );

	Manager::initialize();
}

void GameManager::update( void )
{

}