#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include "Manager.h"
#include <Windows.h>

class GameManager : public Manager
{
	SINGLETON_INSTANCE( GameManager );
public:
	void initialize( void );
	void update( void );

private:


};

#endif