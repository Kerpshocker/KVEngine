#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <DirectXMath.h>
#include "DataTypes.h"
#include "Resources.h"

namespace KVE
{
	namespace GameComponents
	{
		class GameObject
		{
		public:
			GameObject( void );
			GameObject( GameObject* const mount );
			~GameObject();

			DirectX::XMVECTOR* const getPosition( void ) const;
			DirectX::XMVECTOR* const getRotation( void ) const;
			DirectX::XMVECTOR* const getScale( void ) const;

		protected:
			bool				m_Mounted;
			DirectX::XMVECTOR*	m_Position;
			DirectX::XMVECTOR*	m_Rotation;
			DirectX::XMVECTOR*	m_Scale;
		};
	}
}

#endif