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
			m_Position = vPosition;

			m_OABBCorners.frontTopRight = vFrontTopRight;
			m_OABBCorners.frontTopLeft = XMVectorSet( XMVectorGetX( m_OABBCorners.backBottomLeft ), XMVectorGetY( m_OABBCorners.frontTopRight ), XMVectorGetZ( m_OABBCorners.frontTopRight ), 0.0f );
			m_OABBCorners.frontBottomRight = XMVectorSet( XMVectorGetX( m_OABBCorners.frontTopRight ), XMVectorGetY( m_OABBCorners.backBottomLeft ), XMVectorGetZ( m_OABBCorners.frontTopRight ), 0.0f );
			m_OABBCorners.frontBottomLeft = XMVectorSet( XMVectorGetX( m_OABBCorners.backBottomLeft ), XMVectorGetY( m_OABBCorners.backBottomLeft ), XMVectorGetZ( m_OABBCorners.frontTopRight ), 0.0f );

			m_OABBCorners.backTopRight = XMVectorSet( XMVectorGetX( m_OABBCorners.frontTopRight ), XMVectorGetY( m_OABBCorners.frontTopRight ), XMVectorGetZ( m_OABBCorners.backBottomLeft ), 0.0f );
			m_OABBCorners.backTopLeft = XMVectorSet( XMVectorGetX( m_OABBCorners.backBottomLeft ), XMVectorGetY( m_OABBCorners.frontTopRight ), XMVectorGetZ( m_OABBCorners.backBottomLeft ), 0.0f );
			m_OABBCorners.backBottomRight = XMVectorSet( XMVectorGetX( m_OABBCorners.frontTopRight ), XMVectorGetY( m_OABBCorners.backBottomLeft ), XMVectorGetZ( m_OABBCorners.backBottomLeft ), 0.0f );
			m_OABBCorners.backBottomLeft = vBackBottomLeft;

			m_Scale = &XMVectorSet( 1.0f, 1.0f, 1.0f, 0.0f );
			m_Rotation = &XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );

			m_Width = XMVectorGetX( m_OABBCorners.frontTopRight ) - XMVectorGetX( m_OABBCorners.backBottomLeft );
			m_Height = XMVectorGetY( m_OABBCorners.frontTopRight ) - XMVectorGetY( m_OABBCorners.backBottomLeft );
			m_Depth = XMVectorGetZ( m_OABBCorners.frontTopRight ) - XMVectorGetZ( m_OABBCorners.backBottomLeft );

			//may need faces? fr, back, left, right, top, bot

			UpdateCollisionPoints();

			m_Normals = new XMVECTOR[ 6 ];
			m_Normals[ 0 ] = getFrontNormal();
			m_Normals[ 1 ] = getBackNormal();
			m_Normals[ 2 ] = getTopNormal();
			m_Normals[ 3 ] = getBottomNormal();
			m_Normals[ 4 ] = getLeftNormal();
			m_Normals[ 5 ] = getRightNormal();
		}

		OBB::~OBB()
		{
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

			m_CollisionCorners = nullptr;
			m_CollisionCorners = new XMVECTOR[ 8 ];
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
