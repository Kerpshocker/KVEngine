#ifndef ORIENTATION_ALIGNED_BOUNDING_BOX
#define ORIENTATION_ALIGNED_BOUNDING_BOX

#include "BoundingVolume.h"

using namespace KVE::GameComponents;

namespace KVE
{
	namespace Collisions
	{
		class OBB : Collisions::BoundingVolume
		{
		public:
			OBB( );
			OBB( Vector position, Vector frontTopRight, Vector backBottomLeft );
			~OBB( );

			Vector getCollisionFrontTopRight( void ){ return collisionFrontTopRight; };
			Vector getCollisionFrontTopLeft( void ){ return collisionFrontTopLeft; };
			Vector getCollisionFrontBottomRight( void ){ return collisionFrontBottomRight; };
			Vector getCollisionFrontBottomLeft( void ){ return collisionFrontBottomLeft; };
			Vector getCollisionBackTopRight( void ){ return collisionBackTopRight; };
			Vector getCollisionBackTopLeft( void ){ return collisionBackTopLeft; };
			Vector getCollisionBackBottomRight( void ){ return collisionBackBottomRight; };
			Vector getCollisionBackBottomLeft( void ){ return collisionBackBottomLeft; };
			
			Vector getPosition( void ){ return position; };
			Vector getRotation( void ){ return rotation; };
			Vector getScale( void ){ return scale; };

			//set pos, rot, scale and update collision points of OABB
			void setPosition( Vector vPosition );
			void setRotation( Vector vRotation );
			void setScale( Vector vScale );

			f32 getWidth( void ){ return width; };
			f32 getHeight( void ){ return height; };
			f32 getDepth( void ){ return depth; };

			IntersectionValue intersects( BoundingVolume other );

		private:
			UINT indices;						//used for the drawing of the OABB

			Vector frontTopRight;				//front top right corner of OABB
			Vector frontTopLeft;				//front top left corner of OABB
			Vector frontBottomRight;			//front bottom right corner of OABB
			Vector frontBottomLeft;				//front bottom left corner of OABB
			Vector backTopRight;				//back top right corner of OABB
			Vector backTopLeft;					//back top left corner of OABB
			Vector backBottomRight;				//back bottom right corner of OABB
			Vector backBottomLeft;				//back bottom left corner of OABB

			Vector collisionFrontTopRight;		//translated top top right corner of OABB used for collisions
			Vector collisionFrontTopLeft;		//translated top top left corner of OABB used for collisions
			Vector collisionFrontBottomRight;	//translated top bottom right corner of OABB used for collisions
			Vector collisionFrontBottomLeft;	//translated top bottom left corner of OABB used for collisions
			Vector collisionBackTopRight;		//translated back top right corner of OABB used for collisions
			Vector collisionBackTopLeft;		//translated back top left corner of OABB used for collisions
			Vector collisionBackBottomRight;	//translated back bottom right corner of OABB used for collisions
			Vector collisionBackBottomLeft;		//translated back bottom left corner of OABB used for collisions

			Vector position;
			Vector rotation;
			Vector scale;

			f32 width;
			f32 height;
			f32 depth;

			//used to keep in sync with the characteristics of the enclosed object
			void UpdateCollisionPoints( );
		};
	}
}

#endif
