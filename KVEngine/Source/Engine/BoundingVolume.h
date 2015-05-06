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

		class BoundingVolume : GameComponents::GameObject
		{
		public:
			BoundingVolume();
			~BoundingVolume();

			GameComponents::Vector* getCollisionCorners( void ){ return collisionCorners; };
			GameComponents::Vector* getNormals( void ){ return normals; };

			virtual IntersectionValue intersects( BoundingVolume other );

		private:
			GameComponents::Vector* collisionCorners;
			GameComponents::Vector* normals;
		};
	}
}

#endif