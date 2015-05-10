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

		class OBB : BoundingVolume
		{
		public:
			OBB( );
			OBB( DirectX::XMVECTOR* position, DirectX::XMVECTOR frontTopRight, DirectX::XMVECTOR backBottomLeft );
			~OBB( );

			DirectX::XMVECTOR getCollisionFrontTopRight( void ){ return m_OABBCollisionCorners.collisionFrontTopRight; };
			DirectX::XMVECTOR getCollisionFrontTopLeft( void ){ return m_OABBCollisionCorners.collisionFrontTopLeft; };
			DirectX::XMVECTOR getCollisionFrontBottomRight( void ){ return m_OABBCollisionCorners.collisionFrontBottomRight; };
			DirectX::XMVECTOR getCollisionFrontBottomLeft( void ){ return m_OABBCollisionCorners.collisionFrontBottomLeft; };
			DirectX::XMVECTOR getCollisionBackTopRight( void ){ return m_OABBCollisionCorners.collisionBackTopRight; };
			DirectX::XMVECTOR getCollisionBackTopLeft( void ){ return m_OABBCollisionCorners.collisionBackTopLeft; };
			DirectX::XMVECTOR getCollisionBackBottomRight( void ){ return m_OABBCollisionCorners.collisionBackBottomRight; };
			DirectX::XMVECTOR getCollisionBackBottomLeft( void ){ return m_OABBCollisionCorners.collisionBackBottomLeft; };

			f32 getWidth( void ){ return m_Width; };
			f32 getHeight( void ){ return m_Height; };
			f32 getDepth( void ){ return m_Depth; };
			
			DirectX::XMVECTOR* getPosition( void ){ return m_Position; };
			DirectX::XMVECTOR* getRotation( void ){ return m_Rotation; };
			DirectX::XMVECTOR* getScale( void ){ return m_Scale; };

			//set pos, rot, scale and update collision points of OABB
			void setPosition( DirectX::XMVECTOR* vPosition );
			void setRotation( DirectX::XMVECTOR* vRotation );
			void setScale( DirectX::XMVECTOR* vScale );

		private:
			OABBCorners				m_OABBCorners;
			OABBCollisionCorners	m_OABBCollisionCorners;

			UINT					m_IndicesCount;
			UINT					m_Indices;					//used for the drawing of the OABB

			f32						m_Width;
			f32						m_Height;
			f32						m_Depth;

			//used to keep in sync with the characteristics of the enclosed object
			void UpdateCollisionPoints( void );
			//void UpdateDrawingVectors( void );

		protected:
			DirectX::XMVECTOR getFrontNormal( void ){
				DirectX::XMVECTOR normal = DirectX::XMVector3Cross(
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionFrontTopRight, m_OABBCollisionCorners.collisionFrontBottomRight ),
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionFrontBottomLeft, m_OABBCollisionCorners.collisionFrontBottomRight )
					);

				return normal;
			};
			DirectX::XMVECTOR getBackNormal( void ){
				DirectX::XMVECTOR normal = DirectX::XMVector3Cross(
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionBackTopLeft, m_OABBCollisionCorners.collisionBackBottomLeft ),
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionBackBottomRight, m_OABBCollisionCorners.collisionBackBottomLeft )
					);

				return normal;
			};
			DirectX::XMVECTOR getTopNormal( void ){
				DirectX::XMVECTOR normal = DirectX::XMVector3Cross(
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionFrontTopRight, m_OABBCollisionCorners.collisionBackTopLeft ),
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionBackTopRight, m_OABBCollisionCorners.collisionBackTopLeft )
					);

				return normal;
			};
			DirectX::XMVECTOR getBottomNormal( void ){
				DirectX::XMVECTOR normal = DirectX::XMVector3Cross(
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionBackBottomRight, m_OABBCollisionCorners.collisionBackBottomLeft ),
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionFrontBottomRight, m_OABBCollisionCorners.collisionBackBottomLeft )
					);

				return normal;
			};
			DirectX::XMVECTOR getRightNormal( void ){
				DirectX::XMVECTOR normal = DirectX::XMVector3Cross(
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionFrontTopRight, m_OABBCollisionCorners.collisionBackTopRight ),
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionFrontBottomRight, m_OABBCollisionCorners.collisionFrontTopRight )
					);

				return normal;
			};
			DirectX::XMVECTOR getLeftNormal( void ){
				DirectX::XMVECTOR normal = DirectX::XMVector3Cross(
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionFrontTopLeft, m_OABBCollisionCorners.collisionBackTopLeft ),
					DirectX::XMVectorSubtract( m_OABBCollisionCorners.collisionBackTopLeft, m_OABBCollisionCorners.collisionBackBottomLeft )
					);

				return normal;
			};
		};
	}
}

#endif
