#include "MemoryManager.h"
#include <new>

namespace KVE
{
	namespace System
	{
		PageAllocator::PageAllocator( void )
		{
			m_NextFreeByte = 0;
			m_AllocatedPageCount = 1;

			m_AllocatedPages = new Page*[ m_AllocatedPageCount ];
			m_AllocatedPages[ 0 ] = MemoryManager::Instance().allocPage();
		}

		PageAllocator::~PageAllocator( void )
		{
			this->free();
		}

		void* PageAllocator::alloc( const size_t size )
		{
			if ( m_NextFreeByte + size > MemoryManager::PAGE_SIZE )
			{
				// grab a new page
			}

			void* ptr = (char*)m_AllocatedPages[ m_AllocatedPageCount - 1 ]->data + m_NextFreeByte;
			m_NextFreeByte += size;
			return ptr;
		}

		void PageAllocator::free( void )
		{
			for ( unsigned int i = 0; i < m_AllocatedPageCount; i++ )
			{
				MemoryManager::Instance().freePage( m_AllocatedPages[ i ] );
			}
		}

		void PageAllocator::reset( void )
		{
			for ( unsigned int i = 0; i < m_AllocatedPageCount; i++ )
			{
				memset( m_AllocatedPages[ i ]->data, 0, m_NextFreeByte );
			}

			m_NextFreeByte = 0;
		}

		void* operator new( const size_t size, PageAllocator& a )
		{
			return a.alloc( size );;
		}

		void MemoryManager::initialize( void )
		{
			m_Memory = new char[ MEM_POOL_SIZE ];
			memset( m_Memory, 0, MEM_POOL_SIZE );

			m_FreePages = new Page[ PAGE_COUNT ];
			for ( int i = 0; i < PAGE_COUNT; i++ )
			{
				m_FreePages[ i ].data = m_Memory + PAGE_SIZE * i;
				m_FreePages[ i ].next = &m_FreePages[ ( i + 1 ) % PAGE_COUNT ];
			}

			/*Page* head = reinterpret_cast<Page*>( new( m_Memory ) char[ PAGE_SIZE ] );
			m_FreePages = head;

			for ( unsigned int i = 1; i < PAGE_COUNT; i++ )
			{
				head->next = reinterpret_cast<Page*>( new( m_Memory + i * PAGE_SIZE ) char[ PAGE_SIZE ] );
				head = head->next;
			}*/
		}

		void MemoryManager::release( void )
		{
			if ( m_Memory != nullptr )
			{
				delete[] m_Memory;
				m_Memory = nullptr;
			}
		}

		Page* MemoryManager::allocPage( void )
		{
			Page* head = m_FreePages;
			m_FreePages = head->next;
			return head;
		}

		void MemoryManager::freePage( Page* const page )
		{
			page->next = m_FreePages;
			m_FreePages = page;
		}
	}
}