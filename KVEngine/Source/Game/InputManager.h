#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "Manager.h"
#include <hash_map>

enum Keys
{
	KEY_MOVE_FORWARD	= 1,	//right pedal - pos
	KEY_MOVE_BACKWARD	= 2,	//right pedal - neg
	KEY_PITCH_NEG		= 4,	//right joystick forw
	KEY_PITCH_POS		= 8,	//right joystick back
	KEY_YAW_NEG			= 16,	//right joystick left
	KEY_YAW_POS			= 32,	//right joystick right
	KEY_ROLL_NEG		= 64,	//left joystick left
	KEY_ROLL_POS		= 128,	//left joystick right
	KEY_Z_TRIGGER		= 256	//trigger behind right joystick
};

class InputManager : public Manager
{
	SINGLETON_INSTANCE( InputManager );
public:
	void initialize();
	void release();

	bool isKeyPressed(int key);
	void registerKeyPress( int key );
	void registerKeyRelease( int key );

private:
	int keyMap[ 26 ];
	int keyData;
};

#endif

