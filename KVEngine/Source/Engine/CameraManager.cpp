#include "CameraManager.h"

int CameraManager::createNewCamera( const KVE::CameraParams& camParams, const bool makeActive )
{
	if ( numCameras >= MAX_CAMERAS )
	{
		// camera manager already full
		return -1;
	}

	//new ( &cameras[ numCameras++ ] ) KVE::Camera( camParams );
	cameras[ numCameras++ ] = KVE::Camera( camParams );

	if ( makeActive ) activeIndex = numCameras - 1;

	return numCameras;
}

void CameraManager::changeActiveCamera( const uint32_t index )
{
	assert( index < numCameras );

	activeIndex = index;
}

KVE::Camera* const CameraManager::getActiveCamera( void )
{
	assert( numCameras > 0 );

	return &cameras[ activeIndex ];
}