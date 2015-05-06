#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Manager.h"
#include "Camera.h"

#define MAX_CAMERAS 1

class CameraManager : public KVE::Utilities::Manager
{
	SINGLETON_INSTANCE( CameraManager );
public:
	void release( void ) { }

	int createNewCamera( const KVE::Graphics::CameraParams& camParams, const bool makeActive = false );
	void changeActiveCamera( const uint32_t index );

	KVE::Graphics::Camera* const getActiveCamera( void );

private:
	uint32_t numCameras;
	int activeIndex;
	KVE::Graphics::Camera cameras[ MAX_CAMERAS ];

};

#endif