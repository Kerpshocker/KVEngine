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
			m_Corners.frontTopRight = vFrontTopRight;
			m_Corners.frontTopLeft = XMVectorSet( XMVectorGetX( m_Corners.backBottomLeft ), XMVectorGetY( m_Corners.frontTopRight ), XMVectorGetZ( m_Corners.frontTopRight ), 0.0f );
			m_Corners.frontBottomRight = XMVectorSet( XMVectorGetX( m_Corners.frontTopRight ), XMVectorGetY( m_Corners.backBottomLeft ), XMVectorGetZ( m_Corners.frontTopRight ), 0.0f );
			m_Corners.frontBottomLeft = XMVectorSet( XMVectorGetX( m_Corners.backBottomLeft ), XMVectorGetY( m_Corners.backBottomLeft ), XMVectorGetZ( m_Corners.frontTopRight ), 0.0f );

			m_Corners.backTopRight = XMVectorSet( XMVectorGetX( m_Corners.frontTopRight ), XMVectorGetY( m_Corners.frontTopRight ), XMVectorGetZ( m_Corners.backBottomLeft ), 0.0f );
			m_Corners.backTopLeft = XMVectorSet( XMVectorGetX( m_Corners.backBottomLeft ), XMVectorGetY( m_Corners.frontTopRight ), XMVectorGetZ( m_Corners.backBottomLeft ), 0.0f );
			m_Corners.backBottomRight = XMVectorSet( XMVectorGetX( m_Corners.frontTopRight ), XMVectorGetY( m_Corners.backBottomLeft ), XMVectorGetZ( m_Corners.backBottomLeft ), 0.0f );
			m_Corners.backBottomLeft = vBackBottomLeft;

			m_Scale = &XMVectorSet( 1.0f, 1.0f, 1.0f, 0.0f );
			m_Rotation = &XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );

			m_Width = XMVectorGetX( m_Corners.frontTopRight ) - XMVectorGetX( m_Corners.backBottomLeft );
			m_Height = XMVectorGetY( m_Corners.frontTopRight ) - XMVectorGetY( m_Corners.backBottomLeft );
			m_Depth = XMVectorGetZ( m_Corners.frontTopRight ) - XMVectorGetZ( m_Corners.backBottomLeft );

			UpdateCollisionPoints();
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

		XMVECTOR OBB::getSurfaceNormal( XMVECTOR vert1, XMVECTOR vert2, XMVECTOR vert3 )
		{
			//create edges from verts
			XMVECTOR normal;

			XMVECTOR vert1SubVert2 = XMVectorSet(
				XMVectorGetX( vert1 ) - XMVectorGetX( vert2 ),
				XMVectorGetY( vert1 ) - XMVectorGetY( vert2 ),
				XMVectorGetZ( vert1 ) - XMVectorGetZ( vert2 ),
				XMVectorGetW( vert1 ) - XMVectorGetW( vert2 )
				);

			XMVECTOR vert3SubVert2 = XMVectorSet(
				XMVectorGetX( vert3 ) - XMVectorGetX( vert2 ),
				XMVectorGetY( vert3 ) - XMVectorGetY( vert2 ),
				XMVectorGetZ( vert3 ) - XMVectorGetZ( vert2 ),
				XMVectorGetW( vert3 ) - XMVectorGetW( vert2 )
				);

			normal = XMVector3Cross( vert1SubVert2, vert3SubVert2 );

			XMVector3Normalize( normal );

			return normal;
		}
	}
}
