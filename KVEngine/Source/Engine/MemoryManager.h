#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "Singleton.h"

namespace KVE
{
	namespace System
	{
		struct Page
		{
			Page* next;
			void* data;
		};

		class PageAllocator
		{
		public:
			PageAllocator( void );
			~PageAllocator( void );
			void release( void );

			void* alloc( const size_t size );
			void  reset( void );

		private:
			unsigned int m_NextFreeByte;
			unsigned int m_AllocatedPageCount;
			Page** m_AllocatedPages;

		};

		void* operator new( const size_t size, PageAllocator& a );

		class MemoryManager
		{
			friend class PageAllocator;
			SINGLETON_INSTANCE( MemoryManager );

			static const unsigned int MEM_POOL_SIZE = 65536;	// 64MB
			static const unsigned int PAGE_SIZE = 1024;		// 1MB
			static const unsigned int PAGE_COUNT = MEM_POOL_SIZE / PAGE_SIZE;

			char* m_Memory;
			Page* m_FreePages;
			Page* m_PagesHead;

		public:
			void initialize( void );
			void release( void );

		private:
			Page* allocPage( void );
			void  freePage( Page* const page );
		};
	}
}

#endif