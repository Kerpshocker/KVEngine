#ifndef SINGLETON_H
#define SINGLETON_H

namespace KVE
{
	namespace Utilities
	{

#define SINGLETON_INSTANCE( s )		\
public:								\
	static s& Instance()			\
	{								\
		static s instance;			\
		return instance;			\
	}								\
private:							\
	s( void ) { }					\
	s( const s& ) { }				\
	void operator=( const s& ) { }

	}
}

#endif