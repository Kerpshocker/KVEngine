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

			m_CollisionCorners = (XMVECTOR*)_mm_malloc( sizeof( XMVECTOR ) * 8, 16 );
			
			UpdateCollisionPoints();
			
			m_Normals = (XMVECTOR*)_mm_malloc( sizeof( XMVECTOR ) * 6, 16 );
			m_Normals[ 0 ] = getFrontNormal();
			m_Normals[ 1 ] = getBackNormal();
			m_Normals[ 2 ] = getTopNormal();
			m_Normals[ 3 ] = getBottomNormal();
			m_Normals[ 4 ] = getLeftNormal();
			m_Normals[ 5 ] = getRightNormal();

			m_Indices = new UINT[ 8 ];
			m_Indices[ 0 ] = 0;
			m_Indices[ 1 ] = 1;
			m_Indices[ 2 ] = 1;
			m_Indices[ 3 ] = 2;
			m_Indices[ 4 ] = 2;
			m_Indices[ 5 ] = 3;
			m_Indices[ 6 ] = 3;
			m_Indices[ 7 ] = 4;
		}

		OBB::~OBB()
		{
			if ( m_CollisionCorners != nullptr )
			{
				_mm_free( m_CollisionCorners );
				m_CollisionCorners = nullptr;
			}
			if ( m_Normals != nullptr )
			{
				_mm_free( m_Normals );
				m_Normals = nullptr;
			}
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
				XMMatrixScaling( XMVectorGetX( *m_Scale ), XMVectorGetY( *m_Scale ), XMVectorGetZ( *m_Scale ) ) *
				XMMatrixRotationX( XMVectorGetX( *m_Rotation ) ) *
				XMMatrixRotationY( XMVectorGetY( *m_Rotation ) ) *
				XMMatrixRotationZ( XMVectorGetZ( *m_Rotation ) ) *
				XMMatrixTranslation( XMVectorGetX( *m_Position ), XMVectorGetY( *m_Position ), XMVectorGetZ( *m_Position ) 
				);

			m_OABBCollisionCorners.collisionBackTopRight = XMVector3Transform( m_OABBCorners.backTopRight, worldMatrix );
			m_OABBCollisionCorners.collisionBackTopRight = XMVector3Transform( m_OABBCorners.backTopLeft, worldMatrix );
			m_OABBCollisionCorners.collisionBackTopRight = XMVector3Transform( m_OABBCorners.backBottomRight, worldMatrix );
			m_OABBCollisionCorners.collisionBackTopRight = XMVector3Transform( m_OABBCorners.backBottomLeft, worldMatrix );

			m_OABBCollisionCorners.collisionBackTopRight = XMVector3Transform( m_OABBCorners.frontTopRight, worldMatrix );
			m_OABBCollisionCorners.collisionBackTopRight = XMVector3Transform( m_OABBCorners.frontTopLeft, worldMatrix );
			m_OABBCollisionCorners.collisionBackTopRight = XMVector3Transform( m_OABBCorners.frontBottomRight, worldMatrix );
			m_OABBCollisionCorners.collisionBackTopRight = XMVector3Transform( m_OABBCorners.frontBottomLeft, worldMatrix );
			
			m_CollisionCorners[ 0 ] = getCollisionBackTopRight();
			m_CollisionCorners[ 1 ] = getCollisionBackTopLeft();
			m_CollisionCorners[ 2 ] = getCollisionBackBottomRight();
			m_CollisionCorners[ 3 ] = getCollisionBackBottomLeft();
			m_CollisionCorners[ 4 ] = getCollisionFrontTopRight();
			m_CollisionCorners[ 5 ] = getCollisionFrontTopLeft();
			m_CollisionCorners[ 6 ] = getCollisionFrontBottomRight();
			m_CollisionCorners[ 7 ] = getCollisionFrontBottomLeft();
		}
	}
}
