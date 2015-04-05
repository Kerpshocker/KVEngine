#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include "Singleton.h"

class UpdateManager
{
	SINGLETON_INSTANCE( UpdateManager );
public:
	void initialize( void );
	void update( void );

private:


};

#endif