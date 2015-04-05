#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

/*
	This class should move to Engine project, but will start here for ease of beginning this project.
*/

#include "Singleton.h"

class RenderManager
{
	SINGLETON_INSTANCE( RenderManager );
public:
	void initialize( void );
	void render( void );

private:

};

#endif