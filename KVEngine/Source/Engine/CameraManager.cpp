#include "CameraManager.h"
#include "MemoryManager.h"

namespace KVE
{
	static System::PageAllocator* cameraMemory;

	namespace Graphics
	{
		void CameraManager::initialize( void )
		{
			cameraMemory = new System::PageAllocator();
		}

		void CameraManager::release( void )
		{
			if ( cameraMemory != nullptr )
			{
				delete cameraMemory;
				cameraMemory = nullptr;
			}
		}

		int CameraManager::createNewCamera( const CameraParams& camParams, const bool makeActive )
		{
			// is camera manager already full?
			if ( numCameras >= MAX_CAMERAS ) return -1;

			cameras[ numCameras ] = new (cameraMemory->alloc( sizeof( Camera ) )) Camera(camParams);

			if ( makeActive ) activeIndex = numCameras;

			return ++numCameras;
		}

		void CameraManager::changeActiveCamera( const uint32_t index )
		{
			assert( index < numCameras );

			activeIndex = index;
		}

		Camera* const CameraManager::getActiveCamera( void )
		{
			assert( numCameras > 0 );

			return cameras[ activeIndex ];
		}
	}
}