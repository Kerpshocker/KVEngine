#include "OBB.h"

#include <math.h>

using namespace DirectX;

namespace KVE
{
	namespace Collisions
	{
		OBB::OBB( )
		{
		}

		OBB::OBB( XMFLOAT3 vPosition, XMFLOAT3 vFrontTopRight, XMFLOAT3 vBackBottomLeft )
		{
			position = vPosition;
			corners.frontTopRight = vFrontTopRight;
			corners.frontTopLeft = XMFLOAT3{ corners.backBottomLeft.x, corners.frontTopRight.y, corners.frontTopRight.z };
			corners.frontBottomRight = XMFLOAT3{ corners.frontTopRight.x, corners.backBottomLeft.y, corners.frontTopRight.z };
			corners.frontBottomLeft = XMFLOAT3{ corners.backBottomLeft.x, corners.backBottomLeft.y, corners.frontTopRight.z };

			corners.backTopRight = XMFLOAT3{ corners.frontTopRight.x, corners.frontTopRight.y, corners.backBottomLeft.z };
			corners.backTopLeft = XMFLOAT3{ corners.backBottomLeft.x, corners.frontTopRight.y, corners.backBottomLeft.z };
			corners.backBottomRight = XMFLOAT3{ corners.frontTopRight.x, corners.backBottomLeft.y, corners.backBottomLeft.z };
			corners.backBottomLeft = vBackBottomLeft;

			scale = XMFLOAT3{ 1.0f, 1.0f, 1.0f };
			rotation = XMFLOAT3{ 0.0f, 0.0f, 0.0f };

			width = abs( corners.frontTopRight.x - corners.backBottomLeft.x );
			height = abs( corners.frontTopRight.y - corners.backBottomLeft.y );
			depth = abs( corners.frontTopRight.z - corners.backBottomLeft.z );

			UpdateCollisionPoints( );
		}

		OBB::~OBB( )
		{
		}

		void OBB::setPosition( XMFLOAT3 vPosition )
		{
			position = vPosition;
			UpdateCollisionPoints( );
		}

		void OBB::setRotation( XMFLOAT3 vRotation )
		{
			position = vRotation;
			UpdateCollisionPoints( );
		}

		void OBB::setScale( XMFLOAT3 vScale )
		{
			position = vScale;
			UpdateCollisionPoints( );
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
