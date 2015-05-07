#include "OBB.h"

#include <math.h>

namespace KVE
{
	namespace Collisions
	{
		OBB::OBB( )
		{
		}

		OBB::OBB( DirectX::XMFLOAT3 vPosition, DirectX::XMFLOAT3 vFrontTopRight, DirectX::XMFLOAT3 vBackBottomLeft )
		{
			position = vPosition;
			frontTopRight = vFrontTopRight;
			frontTopLeft = DirectX::XMFLOAT3{ backBottomLeft.x, frontTopRight.y, frontTopRight.z };
			frontBottomRight = DirectX::XMFLOAT3{ frontTopRight.x, backBottomLeft.y, frontTopRight.z };
			frontBottomLeft = DirectX::XMFLOAT3{ backBottomLeft.x, backBottomLeft.y, frontTopRight.z };

			backTopRight = DirectX::XMFLOAT3{ frontTopRight.x, frontTopRight.y, backBottomLeft.z };
			backTopLeft = DirectX::XMFLOAT3{ backBottomLeft.x, frontTopRight.y, backBottomLeft.z };
			backBottomRight = DirectX::XMFLOAT3{ frontTopRight.x, backBottomLeft.y, backBottomLeft.z };
			backBottomLeft = vBackBottomLeft;

			scale = DirectX::XMFLOAT3{ 1.0f, 1.0f, 1.0f };
			rotation = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f };

			width = abs( frontTopRight.x - backBottomLeft.x );
			height = abs( frontTopRight.y - backBottomLeft.y );
			depth = abs( frontTopRight.z - backBottomLeft.z );

			UpdateCollisionPoints( );
		}

		OBB::~OBB( )
		{
		}

		void OBB::setPosition( DirectX::XMFLOAT3 vPosition )
		{
			position = vPosition;
			UpdateCollisionPoints( );
		}

		void OBB::setRotation( DirectX::XMFLOAT3 vRotation )
		{
			position = vRotation;
			UpdateCollisionPoints( );
		}

		void OBB::setScale( DirectX::XMFLOAT3 vScale )
		{
			position = vScale;
			UpdateCollisionPoints( );
		}
	}
}
