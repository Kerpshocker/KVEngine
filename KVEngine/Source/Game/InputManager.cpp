#include "InputManager.h"

namespace KVE
{
	namespace Input
	{
		void InputManager::initialize()
		{
			keyData = 0;

			keyMap[ 'W' - 'A' ] = KEY_MOVE_FORWARD;
			keyMap[ 'S' - 'A' ] = KEY_MOVE_BACKWARD;
			keyMap[ 'A' - 'A' ] = KEY_YAW_NEG;
			keyMap[ 'D' - 'A' ] = KEY_YAW_POS;
			keyMap[ 'I' - 'A' ] = KEY_PITCH_NEG;
			keyMap[ 'K' - 'A' ] = KEY_PITCH_POS;
			keyMap[ 'J' - 'A' ] = KEY_ROLL_NEG;
			keyMap[ 'L' - 'A' ] = KEY_ROLL_POS;
			keyMap[ 'Z' - 'A' ] = KEY_Z_TRIGGER;
		};

		void InputManager::release()
		{

		}

		bool InputManager::isKeyPressed( Keys key )
		{
			return keyData & key;
		}

		void InputManager::registerKeyPress( int key )
		{
			keyData |= keyMap[ key - 'A' ];	//or
		}

		void InputManager::registerKeyRelease( int key )
		{
			keyData ^= keyMap[ key - 'A' ];	//exclusive or
		}
	}
}