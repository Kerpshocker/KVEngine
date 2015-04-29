#include "FrameManager.h"
#include <new>

void FrameManager::initialize( void )
{
	m_ReadIndex = 0;
	m_WriteIndex = 0;

	if ( !m_Data ) throw std::bad_alloc();
}

void FrameManager::release( void )
{
	while ( m_ReadIndex != m_WriteIndex )
	{
		m_Data[ m_ReadIndex ].~FrameParams();

		if ( m_ReadIndex++ == MAX_FRAMES )
		{
			m_ReadIndex = 0;
		}
	}

	free( m_Data );
}

bool FrameManager::pushFrame( FrameParams& in )
{
	const uint32_t currentWrite = m_WriteIndex.load();

	uint32_t nextRecord = currentWrite + 1;
	if ( nextRecord == MAX_FRAMES )
	{
		nextRecord = 0;
	}

	if ( nextRecord != m_ReadIndex.load() )
	{
		m_Data[ currentWrite ] = in;
		m_WriteIndex.store( nextRecord );
		return true;
	}

	// queue is full
	return false;
}

bool FrameManager::popFrame( FrameParams& out )
{
	const uint32_t currentRead = m_ReadIndex.load();

	if ( currentRead == m_WriteIndex.load() )
	{
		// queue is empty
		return false;
	}

	uint32_t nextRecord = currentRead + 1;
	if ( nextRecord == MAX_FRAMES )
	{
		nextRecord = 0;
	}

	out = m_Data[ currentRead ];
	m_ReadIndex.store( nextRecord );
	return true;
}

bool FrameManager::isEmpty( void ) const
{
	return m_ReadIndex.load() == m_WriteIndex.load();
}

bool FrameManager::isFull( void ) const
{
	uint32_t nextRecord = m_WriteIndex.load() + 1;
	if ( nextRecord == MAX_FRAMES )
	{
		nextRecord = 0;
	}

	return nextRecord == m_ReadIndex.load();
}