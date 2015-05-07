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
			m_Corners.frontTopLeft = XMVectorSet( XMVectorGetIntX( m_Corners.backBottomLeft ), XMVectorGetIntY( m_Corners.frontTopRight ), XMVectorGetIntZ( m_Corners.frontTopRight ), 0.0f );
			m_Corners.frontBottomRight = XMVectorSet( XMVectorGetIntX( m_Corners.frontTopRight ), XMVectorGetIntY( m_Corners.backBottomLeft ), XMVectorGetIntZ( m_Corners.frontTopRight ), 0.0f );
			m_Corners.frontBottomLeft = XMVectorSet( XMVectorGetIntX( m_Corners.backBottomLeft ), XMVectorGetIntY( m_Corners.backBottomLeft ), XMVectorGetIntZ( m_Corners.frontTopRight ), 0.0f );

			m_Corners.backTopRight = XMVectorSet( XMVectorGetIntX( m_Corners.frontTopRight ), XMVectorGetIntY( m_Corners.frontTopRight ), XMVectorGetIntZ( m_Corners.backBottomLeft ), 0.0f );
			m_Corners.backTopLeft = XMVectorSet( XMVectorGetIntX( m_Corners.backBottomLeft ), XMVectorGetIntY( m_Corners.frontTopRight ), XMVectorGetIntZ( m_Corners.backBottomLeft ), 0.0f );
			m_Corners.backBottomRight = XMVectorSet( XMVectorGetIntX( m_Corners.frontTopRight ), XMVectorGetIntY( m_Corners.backBottomLeft ), XMVectorGetIntZ( m_Corners.backBottomLeft ), 0.0f );
			m_Corners.backBottomLeft = vBackBottomLeft;

			m_Scale = &XMVectorSet( 1.0f, 1.0f, 1.0f, 0.0f );
			m_Rotation = &XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );

			m_Width = XMVectorGetIntX( m_Corners.frontTopRight ) - XMVectorGetIntX( m_Corners.backBottomLeft );
			m_Height = XMVectorGetIntY( m_Corners.frontTopRight ) - XMVectorGetIntY( m_Corners.backBottomLeft );
			m_Depth = XMVectorGetIntZ( m_Corners.frontTopRight ) - XMVectorGetIntZ( m_Corners.backBottomLeft );

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
				XMVectorGetIntX( vert1 ) - XMVectorGetIntX( vert2 ),
				XMVectorGetIntY( vert1 ) - XMVectorGetIntY( vert2 ),
				XMVectorGetIntZ( vert1 ) - XMVectorGetIntZ( vert2 ),
				XMVectorGetIntW( vert1 ) - XMVectorGetIntW( vert2 )
				);

			XMVECTOR vert3SubVert2 = XMVectorSet(
				XMVectorGetIntX( vert3 ) - XMVectorGetIntX( vert2 ),
				XMVectorGetIntY( vert3 ) - XMVectorGetIntY( vert2 ),
				XMVectorGetIntZ( vert3 ) - XMVectorGetIntZ( vert2 ),
				XMVectorGetIntW( vert3 ) - XMVectorGetIntW( vert2 )
				);

			normal = XMVector3Cross( vert1SubVert2, vert3SubVert2 );

			XMVector3Normalize( normal );

			return normal;
		}
	}
}
