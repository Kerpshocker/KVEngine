#include "CameraManager.h"

namespace KVE
{
	namespace Graphics
	{
		int CameraManager::createNewCamera( const CameraParams& camParams, const bool makeActive )
		{
			if ( numCameras >= MAX_CAMERAS )
			{
				// camera manager already full
				return -1;
			}

			//new ( &cameras[ numCameras++ ] ) KVE::Camera( camParams );
			cameras[ numCameras++ ] = Camera( camParams );

			if ( makeActive ) activeIndex = numCameras - 1;

			return numCameras;
		}

		void CameraManager::changeActiveCamera( const uint32_t index )
		{
			assert( index < numCameras );

			activeIndex = index;
		}

		Camera* const CameraManager::getActiveCamera( void )
		{
			assert( numCameras > 0 );

			return &cameras[ activeIndex ];
		}
	}
}