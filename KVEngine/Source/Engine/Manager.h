#ifndef I_MANAGER_H
#define I_MANAGER_H

#include "Singleton.h"

namespace KVE
{
	namespace Utilities
	{
		class Manager
		{
		public:
			Manager( void )
			{
				initialized = false;
			}

			virtual void release( void ) = 0;

			virtual void initialize( void )
			{
				initialized = true;
			}

			bool isInitialized( void ) const
			{
				return initialized;
			}

		protected:
			bool initialized;
		};
	}
}

#endif