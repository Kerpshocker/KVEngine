#include "GameObject.h"

using namespace DirectX;

namespace KVE
{
	namespace GameComponents
	{
		GameObject::GameObject( void ) : m_Mounted( false )
		{
			m_Mounted = false;
			m_Position = (XMVECTOR*)_mm_malloc( sizeof( XMVECTOR ), 16 );
			m_Rotation = (XMVECTOR*) _mm_malloc( sizeof( XMVECTOR ), 16 );
			m_Scale = (XMVECTOR*) _mm_malloc( sizeof( XMVECTOR ), 16 );
		}

		GameObject::GameObject( GameObject* const mount )
		{
			m_Mounted = true;
			m_Position = mount->m_Position;
			m_Rotation = mount->m_Rotation;
			m_Scale = mount->m_Scale;
		}

		GameObject::~GameObject()
		{
			if ( !m_Mounted )
			{
				if ( m_Position != nullptr )
				{
					_mm_free( m_Position );
					m_Position = nullptr;
				}
				if ( m_Rotation != nullptr )
				{
					_mm_free( m_Rotation );
					m_Rotation = nullptr;
				}
				if ( m_Scale != nullptr )
				{
					_mm_free( m_Scale );
					m_Scale = nullptr;
				}
			}
		}

		XMVECTOR* const GameObject::getPosition( void ) const
		{
			return m_Position;
		}

		XMVECTOR* const GameObject::getRotation( void ) const
		{
			return m_Rotation;
		}

		XMVECTOR* const GameObject::getScale( void ) const
		{
			return m_Scale;
		}
	}
}