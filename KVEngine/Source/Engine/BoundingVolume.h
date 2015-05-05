#ifndef BOUNDING_VOLUME
#define BOUNDING_VOLUME

#include "GameObject.h"

namespace KVE
{
    enum IntersectionValue
    {
        CONTAINS,
        INTERSECTS,
        NONE
    };

    class BoundingVolume : GameObject
    {
    public:
        BoundingVolume();
        ~BoundingVolume();

        Vector* getCollisionCorners( void ){ return collisionCorners; };
        Vector* getNormals( void ){ return normals; };

        IntersectionValue intersects( BoundingVolume other );

    private:
        Vector* collisionCorners;
        Vector* normals;
    };

}

#endif