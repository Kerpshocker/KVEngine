#include "OBB.h"

using namespace DirectX;

namespace KVE
{
	namespace Collisions
	{
		OBB::OBB()
		{
		}

		OBB::OBB( XMVECTOR* vPosition, XMVECTOR vFrontTopRight, XMVECTOR vBackBottomLeft )
		{
			*m_Position = *vPosition;

			m_OABBCorners.frontTopRight = vFrontTopRight;
			m_OABBCorners.frontTopLeft = XMVectorSet( XMVectorGetX( vBackBottomLeft ), XMVectorGetY( vFrontTopRight ), XMVectorGetZ( vFrontTopRight ), 0.0f );
			m_OABBCorners.frontBottomRight = XMVectorSet( XMVectorGetX( vFrontTopRight ), XMVectorGetY( vBackBottomLeft ), XMVectorGetZ( vFrontTopRight ), 0.0f );
			m_OABBCorners.frontBottomLeft = XMVectorSet( XMVectorGetX( vBackBottomLeft ), XMVectorGetY( vBackBottomLeft ), XMVectorGetZ( vFrontTopRight ), 0.0f );

			m_OABBCorners.backTopRight = XMVectorSet( XMVectorGetX( vFrontTopRight ), XMVectorGetY( vFrontTopRight ), XMVectorGetZ( vBackBottomLeft ), 0.0f );
			m_OABBCorners.backTopLeft = XMVectorSet( XMVectorGetX( vBackBottomLeft ), XMVectorGetY( vFrontTopRight ), XMVectorGetZ( vBackBottomLeft ), 0.0f );
			m_OABBCorners.backBottomRight = XMVectorSet( XMVectorGetX( vFrontTopRight ), XMVectorGetY( vBackBottomLeft ), XMVectorGetZ( vBackBottomLeft ), 0.0f );
			m_OABBCorners.backBottomLeft = vBackBottomLeft;

			*m_Scale = XMVectorSet( 1.0f, 1.0f, 1.0f, 0.0f );
			*m_Rotation = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );

			m_Width = XMVectorGetX( XMVectorAbs( XMVectorSubtract( m_OABBCorners.frontTopRight, m_OABBCorners.backBottomLeft ) ) );
			m_Height = XMVectorGetY( XMVectorAbs( XMVectorSubtract( m_OABBCorners.frontTopRight, m_OABBCorners.backBottomLeft ) ) );
			m_Depth = XMVectorGetZ( XMVectorAbs( XMVectorSubtract( m_OABBCorners.frontTopRight, m_OABBCorners.backBottomLeft ) ) );

			//may need faces? fr, back, left, right, top, bot
			
			UpdateCollisionPoints();
			
			m_Normals[ 0 ] = getFrontNormal();
			m_Normals[ 1 ] = getBackNormal();
			m_Normals[ 2 ] = getTopNormal();
			m_Normals[ 3 ] = getBottomNormal();
			m_Normals[ 4 ] = getLeftNormal();
			m_Normals[ 5 ] = getRightNormal();

			m_Indices[ 0 ] = 0;
			m_Indices[ 1 ] = 1;
			m_Indices[ 2 ] = 0;
			m_Indices[ 3 ] = 7;
			m_Indices[ 4 ] = 1;
			m_Indices[ 5 ] = 6;
			m_Indices[ 6 ] = 1;
			m_Indices[ 7 ] = 2;
			m_Indices[ 8 ] = 2;
			m_Indices[ 9 ] = 3;
			m_Indices[ 10 ] = 2;
			m_Indices[ 11 ] = 5;
			m_Indices[ 12 ] = 3;
			m_Indices[ 13 ] = 4;
			m_Indices[ 14 ] = 3;
			m_Indices[ 15 ] = 0;
			m_Indices[ 16 ] = 4;
			m_Indices[ 17 ] = 5;
			m_Indices[ 18 ] = 4;
			m_Indices[ 19 ] = 7;
			m_Indices[ 20 ] = 5;
			m_Indices[ 21 ] = 6;
			m_Indices[ 22 ] = 6;
			m_Indices[ 23 ] = 7;
		}

		OBB::~OBB()
		{
				_mm_free( m_CollisionCorners );
			
		}

		void OBB::setPosition( XMVECTOR* vPosition )
		{
			m_Position = vPosition;
			UpdateCollisionPoints();
		}

		void OBB::setRotation( XMVECTOR* vRotation )
		{
			m_Rotation = vRotation;
			UpdateCollisionPoints();
		}

		void OBB::setScale( XMVECTOR* vScale )
		{
			m_Scale = vScale;
			UpdateCollisionPoints();
		}

		void OBB::UpdateCollisionPoints( void )
		{
			XMMATRIX worldMatrix = 
				XMMatrixTranspose(
					XMMatrixScaling( XMVectorGetX( *m_Scale ), XMVectorGetY( *m_Scale ), XMVectorGetZ( *m_Scale ) ) *
					XMMatrixRotationX( XMVectorGetX( *m_Rotation ) ) *
					XMMatrixRotationY( XMVectorGetY( *m_Rotation ) ) *
					XMMatrixRotationZ( XMVectorGetZ( *m_Rotation ) ) *
					XMMatrixTranslation( XMVectorGetX( *m_Position ), XMVectorGetY( *m_Position ), XMVectorGetZ( *m_Position ) 
				));

			m_OABBCollisionCorners.collisionBackTopRight = XMVector3Transform( m_OABBCorners.backTopRight, worldMatrix );
			m_OABBCollisionCorners.collisionBackTopLeft = XMVector3Transform( m_OABBCorners.backTopLeft, worldMatrix );
			m_OABBCollisionCorners.collisionBackBottomRight = XMVector3Transform( m_OABBCorners.backBottomRight, worldMatrix );
			m_OABBCollisionCorners.collisionBackBottomLeft = XMVector3Transform( m_OABBCorners.backBottomLeft, worldMatrix );

			m_OABBCollisionCorners.collisionFrontTopRight = XMVector3Transform( m_OABBCorners.frontTopRight, worldMatrix );
			m_OABBCollisionCorners.collisionFrontTopLeft = XMVector3Transform( m_OABBCorners.frontTopLeft, worldMatrix );
			m_OABBCollisionCorners.collisionFrontBottomRight = XMVector3Transform( m_OABBCorners.frontBottomRight, worldMatrix );
			m_OABBCollisionCorners.collisionFrontBottomLeft = XMVector3Transform( m_OABBCorners.frontBottomLeft, worldMatrix );
			
			m_CollisionCorners[ 0 ] = getCollisionFrontTopRight();
			m_CollisionCorners[ 1 ] = getCollisionFrontTopLeft();
			m_CollisionCorners[ 2 ] = getCollisionFrontBottomLeft();
			m_CollisionCorners[ 3 ] = getCollisionFrontBottomRight();
			m_CollisionCorners[ 4 ] = getCollisionBackBottomRight();
			m_CollisionCorners[ 5 ] = getCollisionBackBottomLeft();
			m_CollisionCorners[ 6 ] = getCollisionBackTopLeft();
			m_CollisionCorners[ 7 ] = getCollisionBackTopRight();
		}

		IntersectionValue OBB::intersects( XMVECTOR instance, XMVECTOR other )
		{
			f32 myX		= abs(XMVectorGetX( instance )); 
			f32 myY		= abs(XMVectorGetY( instance ));
			f32 myZ		= abs(XMVectorGetZ( instance ));

			f32 otherX	= abs(XMVectorGetX( other ));
			f32 otherY	= abs(XMVectorGetY( other ));
			f32 otherZ	= abs(XMVectorGetZ( other ));

			if ( abs(myX - otherX) <= m_Width && abs(myY - otherY) <= m_Height && abs(myZ - otherZ) <= m_Depth )
				return INTERSECTS;

			return NONE;
		}
	}
}
