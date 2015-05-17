#include "FrameManager.h"
#include "RenderManager.h"
#include <new>

namespace KVE
{
	namespace Graphics
	{
		void FrameManager::initialize( const UINT buffersPerFrame )
		{
			m_FrameReadIndex = 0;
			m_FrameWriteIndex = 0;
			m_AllocReadIndex = 0;
			m_AllocWriteIndex = 0;
			m_Writing = false;

			if ( !m_Data ) throw std::bad_alloc();

			int i;
			for ( i = 0; i < MAX_FRAMES; i++ )
			{
				m_Data[ i ].InstanceStrides = new UINT[ buffersPerFrame ];
				m_Data[ i ].InstanceCounts  = new UINT[ buffersPerFrame ];
				m_Data[ i ].InstanceOffsets	= new UINT[ buffersPerFrame ];
			}

			/*for ( i = 0; i < MAX_QUEUED; i++ )
			{
				m_FrameAllocators[i].al
			}*/
		}

		void FrameManager::release( void )
		{

		}

		bool FrameManager::openFrame( FrameParams** out )
		{
			if ( m_Writing ) return false;

			const uint32_t currentWrite = m_FrameWriteIndex.load( std::memory_order_relaxed );

			uint32_t nextRecord = currentWrite + 1;
			if ( nextRecord == MAX_FRAMES )
			{
				nextRecord = 0;
			}

			if ( nextRecord != m_FrameReadIndex.load( std::memory_order_acquire ) )
			{
				( *out ) = &m_Data[ currentWrite ];

				( *out )->Allocator = &m_FrameAllocators[ m_AllocWriteIndex ];

				return ( m_Writing = true );
			}

			// queue is full
			return ( m_Writing = false );
		}

		bool FrameManager::closeFrame( FrameParams** in )
		{
			if ( !m_Writing ) return false;

			( *in ) = nullptr;

			const uint32_t currentFrameWrite = m_FrameWriteIndex.load( std::memory_order_relaxed );
			uint32_t nextFrameRecord = currentFrameWrite + 1;
			if ( nextFrameRecord == MAX_FRAMES )
			{
				nextFrameRecord = 0;
			}
			m_FrameWriteIndex.store( nextFrameRecord, std::memory_order_release );

			const uint32_t currentAllocWrite = m_AllocWriteIndex.load( std::memory_order_relaxed );
			uint32_t nextAllocRecord = currentAllocWrite + 1;
			if ( nextAllocRecord == MAX_QUEUED )
			{
				nextAllocRecord = 0;
			}
			m_AllocWriteIndex.store( nextAllocRecord, std::memory_order_release );

			return !( m_Writing = false );
		}

		bool FrameManager::readNextFrame( FrameParams** const out )
		{
			const uint32_t currentFrameRead = m_FrameReadIndex.load( std::memory_order_relaxed );

			if ( currentFrameRead == m_FrameWriteIndex.load( std::memory_order_acquire ) )
			{
				// queue is empty
				return false;
			}

			uint32_t nextFrameRecord = currentFrameRead + 1;
			if ( nextFrameRecord == MAX_FRAMES )
			{
				nextFrameRecord = 0;
			}

			( *out ) = &m_Data[ currentFrameRead ];
			m_FrameReadIndex.store( nextFrameRecord, std::memory_order_release );

			const uint32_t currentAllocRead = m_AllocReadIndex.load( std::memory_order_relaxed );
			uint32_t nextAllocRecord = currentAllocRead + 1;
			if ( nextAllocRecord == MAX_FRAMES )
			{
				nextAllocRecord = 0;
			}

			( *out )->Allocator = &m_FrameAllocators[ m_AllocReadIndex ];
			m_AllocReadIndex.store( nextAllocRecord, std::memory_order_release );

			return true;
		}

		bool FrameManager::isWriteReady( void ) const
		{
			// to the left ot MAX_QUEUED in the return is a modulo equation that works with negative values
			const int diff = (INT) m_FrameWriteIndex - (INT) m_FrameReadIndex;
			return !m_Writing && ( ( diff % MAX_FRAMES + MAX_FRAMES ) % MAX_FRAMES <= MAX_QUEUED );
		}

		bool FrameManager::isReadReady( void ) const
		{
			return m_FrameReadIndex != m_FrameWriteIndex;
		}
	}
}