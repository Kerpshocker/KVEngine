#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include "Singleton.h"
#include <Windows.h>

class UpdateManager
{
	SINGLETON_INSTANCE( UpdateManager );
public:
	void initialize( HINSTANCE appInstance );
	void update( void );

private:


};

#endif