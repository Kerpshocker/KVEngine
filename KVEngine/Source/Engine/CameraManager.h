#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Manager.h"
#include "Camera.h"

#define MAX_CAMERAS 1

class CameraManager : public Manager
{
	SINGLETON_INSTANCE( CameraManager );
public:
	void release( void ) { }

	int createNewCamera( const KVE::CameraParams& camParams, const bool makeActive = false );
	void changeActiveCamera( const uint32_t index );

	KVE::Camera* const getActiveCamera( void );

private:
	uint32_t numCameras;
	int activeIndex;
	KVE::Camera cameras[ MAX_CAMERAS ];

};

#endif