#include "FrameManager.h"
#include <new>

namespace KVE
{
	namespace Graphics
	{
		void FrameManager::initialize( void )
		{
			m_ReadIndex = 0;
			m_WriteIndex = 0;
			m_Writing = false;

			if ( !m_Data ) throw std::bad_alloc();
		}

		void FrameManager::release( void )
		{

		}

		bool FrameManager::openFrame( FrameParams** out )
		{
			if ( m_Writing ) return false;

			const uint32_t currentWrite = m_WriteIndex.load( std::memory_order_relaxed );

			uint32_t nextRecord = currentWrite + 1;
			if ( nextRecord == MAX_FRAMES )
			{
				nextRecord = 0;
			}

			if ( nextRecord != m_ReadIndex.load( std::memory_order_acquire ) )
			{
				( *out ) = &m_Data[ currentWrite ];
				return ( m_Writing = true );
			}

			// queue is full
			return ( m_Writing = false );
		}

		bool FrameManager::closeFrame( FrameParams** in )
		{
			if ( !m_Writing ) return false;

			( *in ) = nullptr;

			const uint32_t currentWrite = m_WriteIndex.load( std::memory_order_relaxed );

			uint32_t nextRecord = currentWrite + 1;
			if ( nextRecord == MAX_FRAMES )
			{
				nextRecord = 0;
			}

			m_WriteIndex.store( nextRecord, std::memory_order_release );
			return !( m_Writing = false );
		}

		bool FrameManager::readNextFrame( const FrameParams** const out )
		{
			const uint32_t currentRead = m_ReadIndex.load( std::memory_order_relaxed );

			if ( currentRead == m_WriteIndex.load( std::memory_order_acquire ) )
			{
				// queue is empty
				return false;
			}

			uint32_t nextRecord = currentRead + 1;
			if ( nextRecord == MAX_FRAMES )
			{
				nextRecord = 0;
			}

			( *out ) = &m_Data[ currentRead ];
			m_ReadIndex.store( nextRecord, std::memory_order_release );
			return true;
		}

		bool FrameManager::isWriteReady( void ) const
		{
			// to the left ot MAX_QUEUED in the return is a modulo equation that works with negative values
			const int diff = (INT)m_WriteIndex - (INT)m_ReadIndex;
			return !m_Writing && ( ( diff % MAX_FRAMES + MAX_FRAMES ) % MAX_FRAMES <= MAX_QUEUED );
		}

		bool FrameManager::isReadReady( void ) const
		{
			return m_ReadIndex != m_WriteIndex;
		}
	}
}