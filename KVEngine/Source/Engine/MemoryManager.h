#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "Singleton.h"

namespace KVE
{
	namespace System
	{
		class Allocator
		{
		public:
			Allocator( void );
			~Allocator( void );

			void* alloc( const size_t size );
			void  free( void* const ptr );

		private:
			unsigned int m_NextFreeByte;
			void* m_Page;

		};

		void* operator new( const size_t size, Allocator& a );

		class MemoryManager
		{
			friend class Allocator;
			SINGLETON_INSTANCE( MemoryManager );

			const unsigned int MEM_POOL_SIZE = 65536;	// 64MB
			const unsigned int PAGE_SIZE = 1024;		// 1MB
			const unsigned int PAGE_COUNT = MEM_POOL_SIZE / PAGE_SIZE;

			struct Page
			{
				Page* next;
			};

			char* m_Memory;
			Page* m_FreePages;

		public:
			void initialize( void );
			void release( void );

		private:
			void* allocPage( void );
			void  freePage( void* const ptr );
		};
	}
}

#endif