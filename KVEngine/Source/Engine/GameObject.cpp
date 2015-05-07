#include "GameObject.h"

using namespace DirectX;

namespace KVE
{
	namespace GameComponents
	{
		GameObject::GameObject( void )
			: m_Position( new XMVECTOR() ), m_Rotation( new XMVECTOR() ),
			m_Scale( new XMVECTOR() ), m_Mounted( false )
		{
			
		}

		GameObject::GameObject( GameObject* const mount )
			: m_Position( mount->m_Position ), m_Rotation( mount->m_Rotation ),
			m_Scale( mount->m_Scale ), m_Mounted( true )
		{
			
		}

		GameObject::~GameObject()
		{
			if ( m_Mounted ) return;
			
			if ( m_Position != nullptr ) delete m_Position;
			if ( m_Rotation != nullptr ) delete m_Rotation;
			if ( m_Scale != nullptr ) delete m_Scale;
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