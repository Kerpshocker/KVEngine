#include "OBB.h"

#include <math.h>

namespace KVE
{
	namespace Collisions
	{
		OBB::OBB( )
		{
		}

		OBB::OBB( Vector vPosition, Vector vFrontTopRight, Vector vBackBottomLeft )
		{
			position = vPosition;
			frontTopRight = vFrontTopRight;
			frontTopLeft = Vector{ backBottomLeft.x, frontTopRight.y, frontTopRight.z };
			frontBottomRight = Vector{ frontTopRight.x, backBottomLeft.y, frontTopRight.z };
			frontBottomLeft = Vector{ backBottomLeft.x, backBottomLeft.y, frontTopRight.z };

			backTopRight = Vector{ frontTopRight.x, frontTopRight.y, backBottomLeft.z };
			backTopLeft = Vector{ backBottomLeft.x, frontTopRight.y, backBottomLeft.z };
			backBottomRight = Vector{ frontTopRight.x, backBottomLeft.y, backBottomLeft.z };
			backBottomLeft = vBackBottomLeft;

			scale = Vector{ 1.0f, 1.0f, 1.0f };
			rotation = Vector{ 0.0f, 0.0f, 0.0f };

			width = abs( frontTopRight.x - backBottomLeft.x );
			height = abs( frontTopRight.y - backBottomLeft.y );
			depth = abs( frontTopRight.z - backBottomLeft.z );

			UpdateCollisionPoints( );
		}

		OBB::~OBB( )
		{
		}

		void OBB::setPosition( Vector vPosition )
		{
			position = vPosition;
			UpdateCollisionPoints( );
		}

		void OBB::setRotation( Vector vRotation )
		{
			position = vRotation;
			UpdateCollisionPoints( );
		}

		void OBB::setScale( Vector vScale )
		{
			position = vScale;
			UpdateCollisionPoints( );
		}
	}
}
