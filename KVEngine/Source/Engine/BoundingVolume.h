#ifndef BOUNDING_VOLUME
#define BOUNDING_VOLUME

#include "GameObject.h"

namespace KVE
{
	namespace Collisions
	{
		enum IntersectionValue
		{
			CONTAINS,
			INTERSECTS,
			NONE
		};

		class BoundingVolume : public GameComponents::GameObject
		{
		public:
			BoundingVolume();
			~BoundingVolume();

			DirectX::XMFLOAT3* getCollisionCorners( void ){ return collisionCorners; };
			DirectX::XMFLOAT3* getNormals( void ){ return normals; };

			virtual IntersectionValue intersects( BoundingVolume other );

		private:
			DirectX::XMFLOAT3* collisionCorners;
			DirectX::XMFLOAT3* normals;
		};
	}
}

#endif