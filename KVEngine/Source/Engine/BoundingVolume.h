#ifndef BOUNDING_VOLUME
#define BOUNDING_VOLUME

#include "GameObject.h"

namespace KVE
{
	namespace Collisions
	{
		class BoundingVolume : public GameComponents::GameObject
		{
		public:
			BoundingVolume();
			~BoundingVolume();

			DirectX::XMVECTOR* getCollisionCorners( void ){ return m_CollisionCorners; };
			DirectX::XMVECTOR* getNormals( void ){ return m_Normals; };

		protected:
			DirectX::XMVECTOR* m_CollisionCorners;
			DirectX::XMVECTOR* m_Normals;
		};
	}
}

#endif