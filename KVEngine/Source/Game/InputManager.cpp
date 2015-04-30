#include "InputManager.h"

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

void InputManager::keyPressed( int key )
{
	keyData |= keyMap[ key - 'A' ];	//or
}

void InputManager::keyReleased( int key )
{
	keyData ^= keyMap[ key - 'A' ];	//exclusive or
}