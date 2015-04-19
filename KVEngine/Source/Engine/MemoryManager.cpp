#include "MemoryManager.h"
#include <new>

Allocator::Allocator( void )
{
	m_NextFreeByte = 0;
	m_Page = MemoryManager::Instance().allocPage();
}

Allocator::~Allocator( void )
{
	MemoryManager::Instance().freePage( m_Page );
}

void* Allocator::alloc( const size_t size )
{
	void* ptr = new ( (char*)m_Page + m_NextFreeByte ) char[ size ];
	m_NextFreeByte += size;
	return ptr;
}

void Allocator::free( void* const ptr )
{
	
}

void* operator new( const size_t size, Allocator& a )
{
	return a.alloc( size );;
}

void MemoryManager::initialize( void )
{
	m_Memory = new char[ MEM_POOL_SIZE ];

	Page* head = reinterpret_cast<Page*>( new( m_Memory ) char[ PAGE_SIZE ] );
	m_FreePages = head;

	for ( unsigned int i = 1; i < PAGE_COUNT; i++ )
	{
		head->next = reinterpret_cast<Page*>( new( m_Memory + i * PAGE_SIZE ) char[ PAGE_SIZE ] );
		head = head->next;
	}
}

void MemoryManager::release( void )
{
	if ( m_Memory != nullptr )
	{
		delete[] m_Memory;
		m_Memory = nullptr;
	}
}

void* MemoryManager::allocPage( void )
{
	Page* head = m_FreePages;
	m_FreePages = head->next;
	return head;
}

void MemoryManager::freePage( void* const ptr )
{
	Page* head = static_cast<Page*>( ptr );
	head->next = m_FreePages;
	m_FreePages = head;
}