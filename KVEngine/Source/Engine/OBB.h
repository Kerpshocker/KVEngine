#ifndef ORIENTATION_ALIGNED_BOUNDING_BOX
#define ORIENTATION_ALIGNED_BOUNDING_BOX

#include "BoundingVolume.h"

namespace KVE
{
	namespace Collisions
	{
		struct OABBCorners
		{
			DirectX::XMVECTOR frontTopRight;				//front top right corner of OABB
			DirectX::XMVECTOR frontTopLeft;					//front top left corner of OABB
			DirectX::XMVECTOR frontBottomRight;				//front bottom right corner of OABB
			DirectX::XMVECTOR frontBottomLeft;				//front bottom left corner of OABB
			DirectX::XMVECTOR backTopRight;					//back top right corner of OABB
			DirectX::XMVECTOR backTopLeft;					//back top left corner of OABB
			DirectX::XMVECTOR backBottomRight;				//back bottom right corner of OABB
			DirectX::XMVECTOR backBottomLeft;				//back bottom left corner of OABB
		};

		struct OABBCollisionCorners
		{
			DirectX::XMVECTOR collisionFrontTopRight;		//translated top top right corner of OABB used for collisions
			DirectX::XMVECTOR collisionFrontTopLeft;		//translated top top left corner of OABB used for collisions
			DirectX::XMVECTOR collisionFrontBottomRight;	//translated top bottom right corner of OABB used for collisions
			DirectX::XMVECTOR collisionFrontBottomLeft;		//translated top bottom left corner of OABB used for collisions
			DirectX::XMVECTOR collisionBackTopRight;		//translated back top right corner of OABB used for collisions
			DirectX::XMVECTOR collisionBackTopLeft;			//translated back top left corner of OABB used for collisions
			DirectX::XMVECTOR collisionBackBottomRight;		//translated back bottom right corner of OABB used for collisions
			DirectX::XMVECTOR collisionBackBottomLeft;		//translated back bottom left corner of OABB used for collisions
		};

		struct OABBNormals
		{
			DirectX::XMVECTOR frontNormal;
			DirectX::XMVECTOR backNormal;
			DirectX::XMVECTOR topNormal;
			DirectX::XMVECTOR bottomNormal;
			DirectX::XMVECTOR rightNormal;
			DirectX::XMVECTOR leftNormal;
		};

		class OBB : BoundingVolume
		{
		public:
			OBB( );
			OBB( DirectX::XMVECTOR* position, DirectX::XMVECTOR frontTopRight, DirectX::XMVECTOR backBottomLeft );
			~OBB( );

			DirectX::XMVECTOR getCollisionFrontTopRight( void ){ return m_CollisionCorners.collisionFrontTopRight; };
			DirectX::XMVECTOR getCollisionFrontTopLeft( void ){ return m_CollisionCorners.collisionFrontTopLeft; };
			DirectX::XMVECTOR getCollisionFrontBottomRight( void ){ return m_CollisionCorners.collisionFrontBottomRight; };
			DirectX::XMVECTOR getCollisionFrontBottomLeft( void ){ return m_CollisionCorners.collisionFrontBottomLeft; };
			DirectX::XMVECTOR getCollisionBackTopRight( void ){ return m_CollisionCorners.collisionBackTopRight; };
			DirectX::XMVECTOR getCollisionBackTopLeft( void ){ return m_CollisionCorners.collisionBackTopLeft; };
			DirectX::XMVECTOR getCollisionBackBottomRight( void ){ return m_CollisionCorners.collisionBackBottomRight; };
			DirectX::XMVECTOR getCollisionBackBottomLeft( void ){ return m_CollisionCorners.collisionBackBottomLeft; };

			/*DirectX::XMVECTOR getFrontNormal( void ){ 
				return getSurfaceNormal(
					m_CollisionCorners.collisionFrontTopRight, 
					m_CollisionCorners.collisionFrontBottomRight, 
					m_CollisionCorners.collisionFrontBottomLeft ); 
			};
			DirectX::XMVECTOR getBackNormal( void ){ 
				return getSurfaceNormal(
					m_CollisionCorners.collisionBackTopLeft,
					m_CollisionCorners.collisionBackBottomLeft,
					m_CollisionCorners.collisionBackBottomRight );
			};
			DirectX::XMVECTOR getTopNormal( void ){
				return getSurfaceNormal(
					m_CollisionCorners.collisionFrontTopRight,
					m_CollisionCorners.collisionBackTopLeft,
					m_CollisionCorners.collisionBackTopRight );
			};
			DirectX::XMVECTOR getBottomNormal( void ){
				return getSurfaceNormal(
					m_CollisionCorners.collisionBackBottomRight,
					m_CollisionCorners.collisionBackBottomLeft,
					m_CollisionCorners.collisionFrontBottomRight );
			};
			DirectX::XMVECTOR getRightNormal( void ){
				return getSurfaceNormal(
					m_CollisionCorners.collisionFrontTopRight,
					m_CollisionCorners.collisionBackTopRight,
					m_CollisionCorners.collisionFrontBottomRight );
			};
			DirectX::XMVECTOR getLeftNormal( void ){
				return getSurfaceNormal(
					m_CollisionCorners.collisionFrontTopLeft,
					m_CollisionCorners.collisionBackTopLeft,
					m_CollisionCorners.collisionBackBottomRight );
			};*/

			
			DirectX::XMVECTOR* getPosition( void ){ return m_Position; };
			DirectX::XMVECTOR* getRotation( void ){ return m_Rotation; };
			DirectX::XMVECTOR* getScale( void ){ return m_Scale; };

			//set pos, rot, scale and update collision points of OABB
			void setPosition( DirectX::XMVECTOR* vPosition );
			void setRotation( DirectX::XMVECTOR* vRotation );
			void setScale( DirectX::XMVECTOR* vScale );

			f32 getWidth( void ){ return m_Width; };
			f32 getHeight( void ){ return m_Height; };
			f32 getDepth( void ){ return m_Depth; };

			IntersectionValue intersects( BoundingVolume other );

		private:
			OABBCorners				m_Corners;
			OABBCollisionCorners	m_CollisionCorners;

			UINT					m_Indices;					//used for the drawing of the OABB

			f32						m_Width;
			f32						m_Height;
			f32						m_Depth;

			//used to keep in sync with the characteristics of the enclosed object
			void UpdateCollisionPoints( );

			//returns the normal to a polygons surface, in which the polygon defined by three vectors
			DirectX::XMVECTOR getSurfaceNormal( DirectX::XMVECTOR vert1, DirectX::XMVECTOR vert2, DirectX::XMVECTOR vert3 );

		protected:
			
		};
	}
}

#endif
