#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include "Manager.h"
#include <Windows.h>

class UpdateManager : public Manager
{
	SINGLETON_INSTANCE( UpdateManager );
public:
	void initialize( HINSTANCE appInstance );
	void update( void );

private:


};

#endif